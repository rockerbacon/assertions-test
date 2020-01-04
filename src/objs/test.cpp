#include "test.h"
#include <stopwatch/stopwatch.h>
#include <unordered_map>
#include "low_level_error_handler.h"
#include <csetjmp>

using namespace std;
using namespace test;

list<parallel_tools::complex_atomic<unique_ptr<observer>>> test::observers;

unsigned test::elements_discovered = 0;

std::atomic<unsigned> test::successful_tests_count(0);
std::atomic<unsigned> test::failed_tests_count(0);

parallel_tools::thread_pool test::tests_pool(thread::hardware_concurrency());
std::list<std::future<void>> test::tests_futures;

assert_failed::assert_failed (const string &message)
	:	message(message)
{}

const char* assert_failed::what(void) const noexcept {
	return this->message.c_str();
}

void test::queue_test_for_execution (const string &test_case_description, unsigned row_in_terminal, const test_case& test) {
	auto test_future = test::tests_pool.exec([=]() {
		jmp_buf jump_buffer;
		string low_level_error_message;

		test::setup_signal_handlers(&low_level_error_message, &jump_buffer);

		notify_test_case_execution_begun(test_case_description, row_in_terminal);

		chrono::high_resolution_clock::duration test_duration;
		stopwatch stopwatch;
		try {
			if (!setjmp(jump_buffer)) {
				test();
				test_duration = stopwatch.total_time();
				notify_test_case_succeeded(test_case_description, row_in_terminal, test_duration);
			} else {
				throw assert_failed(low_level_error_message);
			}
		} catch (const exception &e) {
			test_duration = stopwatch.total_time();
			notify_test_case_failed(test_case_description, row_in_terminal, test_duration, e.what());
		}
	});
	test::tests_futures.emplace_back(std::move(test_future));
}

void test::notify_tests_begun() {
	for (auto& observer : observers) {
		observer.access([](auto& observer) {
			observer->tests_begun();
		});
	}
}

void test::notify_test_suite_block_begun(const string& test_suite_description) {
	for (auto& observer : observers) {
		observer.access([&](auto& observer) {
			observer->test_suite_block_begun(test_suite_description);
		});
	}
}

void test::notify_test_case_discovered(const string& test_case_description) {
	for (auto& observer : observers) {
		observer.access([&](auto& observer) {
			observer->test_case_discovered(test_case_description);
		});
	}
}

void test::notify_test_suite_block_ended() {
	for (auto& observer : observers) {
		observer.access([](auto& observer) {
			observer->test_suite_block_ended();
		});
	}
}

void test::notify_test_case_execution_begun(const string& test_case_description, unsigned row) {
	for (auto& observer : observers) {
		observer.access([&](auto& observer) {
			observer->test_case_execution_begun(test_case_description, row);
		});
	}
}

void test::notify_test_case_failed(
	const string& test_case_description,
	unsigned row,
	std::chrono::high_resolution_clock::duration test_duration,
	const string& reason
) {
	test::failed_tests_count++;
	for (auto& observer : observers) {
		observer.access([&](auto& observer) {
			observer->test_case_failed(test_case_description, row, test_duration, reason);
		});
	}
}

void test::notify_test_case_succeeded(
	const string& test_case_description,
	unsigned row,
	std::chrono::high_resolution_clock::duration test_duration
) {
	test::successful_tests_count++;
	for (auto& observer: observers) {
		observer.access([&](auto& observer) {
			observer->test_case_succeeded(test_case_description, row, test_duration);
		});
	}
}

void test::notify_tests_ended(unsigned successful_tests, unsigned failed_tests) {
	for (auto& observer: observers) {
		observer.access([&](auto& observer) {
			observer->tests_ended(successful_tests, failed_tests);
		});
	}
}

void test::wait_tests() {
	for (auto& future : tests_futures) {
		future.wait();
	}
}

