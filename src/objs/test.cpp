#include "test.h"
#include <stopwatch/stopwatch.h>
#include <unordered_map>
#include "low_level_error_handler.h"
#include <csetjmp>

#define SUCCESS_TEXT_STYLE ::terminal::stylize_color(::terminal::bright(::terminal::color_style::GREEN))
#define FAILURE_TEXT_STYLE ::terminal::stylize_color(::terminal::bright(::terminal::color_style::RED))

using namespace std;
using namespace test;

list<parallel::atomic<observer*>> test::observers;

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
		stopwatch stopwatch;
		chrono::high_resolution_clock::duration test_duration;
		jmp_buf jump_buffer;
		string low_level_error_message;

		test::setup_signal_handlers(&low_level_error_message, &jump_buffer);

		for (auto& observer : test::observers) {
			observer->test_case_execution_begun(test_case_description, row_in_terminal);
		}
		try {
			if (!setjmp(jump_buffer)) {
				test();
				test_duration = stopwatch.total_time();
				test::successful_tests_count++;
				for (auto& observer : test::observers) {
					observer->test_case_succeeded(test_case_description, row_in_terminal, test_duration);
				}
			} else {
				throw assert_failed(low_level_error_message);
			}
		} catch (const exception &e) {
			test_duration = stopwatch.total_time();
			test::failed_tests_count++;
			for (auto& observer : test::observers) {
				observer->test_case_failed(test_case_description, row_in_terminal, test_duration, e.what());
			}
		}
	});
	test::tests_futures.emplace_back(std::move(test_future));
}

