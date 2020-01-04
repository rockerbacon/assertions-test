#include "tests_manager.h"

#include <iostream>
#include <stopwatch/stopwatch.h>

#include "observers/json_logger.h"
#include "observers/live_terminal.h"
#include "signal_handler/signal_handler.h"
#include "exceptions/assert_failed.h"

using namespace test;
using namespace std;

tests_manager::tests_manager () :
	tests_pool(std::thread::hardware_concurrency())
{
	test::handle_signal(SIGSEGV);
	test::handle_signal(SIGABRT);

	observers.emplace_back(new live_terminal);
	observers.emplace_back(new json_logger(cerr));
	notify_tests_begun();
}

tests_manager::~tests_manager () {
	wait_tests();
	notify_tests_ended(successful_tests_count, failed_tests_count);
	exit(failed_tests_count);
}

void tests_manager::queue_test_for_execution (const string &test_case_description, unsigned row_in_terminal, const function<void()>& test) {
	auto test_future = tests_pool.exec([=]() {
		string low_level_error_message;

		notify_test_case_execution_begun(test_case_description, row_in_terminal);

		chrono::high_resolution_clock::duration test_duration;
		stopwatch stopwatch;
		try {
			test();
			test_duration = stopwatch.total_time();
			notify_test_case_succeeded(test_case_description, row_in_terminal, test_duration);
		} catch (const exception &e) {
			test_duration = stopwatch.total_time();
			notify_test_case_failed(test_case_description, row_in_terminal, test_duration, e.what());
		}
	});
	tests_futures.emplace_back(std::move(test_future));
}


void tests_manager::notify_tests_begun() {
	for (auto& observer : observers) {
		observer.access([](auto& observer) {
			observer->tests_begun();
		});
	}
}

void tests_manager::notify_test_suite_block_begun(const string& test_suite_description) {
	for (auto& observer : observers) {
		observer.access([&](auto& observer) {
			observer->test_suite_block_begun(test_suite_description);
		});
	}
}

void tests_manager::notify_test_case_discovered(const string& test_case_description) {
	for (auto& observer : observers) {
		observer.access([&](auto& observer) {
			observer->test_case_discovered(test_case_description);
		});
	}
}

void tests_manager::notify_test_suite_block_ended() {
	for (auto& observer : observers) {
		observer.access([](auto& observer) {
			observer->test_suite_block_ended();
		});
	}
}

void tests_manager::notify_test_case_execution_begun(const string& test_case_description, unsigned row) {
	for (auto& observer : observers) {
		observer.access([&](auto& observer) {
			observer->test_case_execution_begun(test_case_description, row);
		});
	}
}

void tests_manager::notify_test_case_failed(
	const string& test_case_description,
	unsigned row,
	std::chrono::high_resolution_clock::duration test_duration,
	const string& reason
) {
	failed_tests_count++;
	for (auto& observer : observers) {
		observer.access([&](auto& observer) {
			observer->test_case_failed(test_case_description, row, test_duration, reason);
		});
	}
}

void tests_manager::notify_test_case_succeeded(
	const string& test_case_description,
	unsigned row,
	std::chrono::high_resolution_clock::duration test_duration
) {
	successful_tests_count++;
	for (auto& observer: observers) {
		observer.access([&](auto& observer) {
			observer->test_case_succeeded(test_case_description, row, test_duration);
		});
	}
}

void tests_manager::notify_tests_ended(unsigned successful_tests, unsigned failed_tests) {
	for (auto& observer: observers) {
		observer.access([&](auto& observer) {
			observer->tests_ended(successful_tests, failed_tests);
		});
	}
}

void tests_manager::wait_tests() {
	for (auto& future : tests_futures) {
		future.wait();
	}
}

