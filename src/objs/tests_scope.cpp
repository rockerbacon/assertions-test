#include "tests_scope.h"

#include <iostream>
#include <stopwatch/stopwatch.h>

#include "observers/json_logger.h"
#include "observers/static_terminal.h"
#include "signal_handler/signal_handler.h"
#include "exceptions/assert_failed.h"

using namespace test;
using namespace std;

tests_scope::tests_scope () :
	tests_pool(std::thread::hardware_concurrency())
{
	test::handle_signal(SIGSEGV);
	test::handle_signal(SIGABRT);

	observers.emplace_back(new static_terminal);
	observers.emplace_back(new json_logger(cerr));
	for (auto& observer : observers) observer.access([](auto& observer) {
		observer->tests_begun();
	});
}

tests_scope::~tests_scope () {
	wait_tests();

	for (auto& observer : observers) observer.access([this](auto& observer) {
		observer->tests_ended(failed_tests_count.load(), successful_tests_count.load());
	});

	exit(failed_tests_count);
}

void tests_scope::wait_tests() {
	for (auto& test_suite : test_suites) {
		for (auto& test_case : test_suite.test_cases) {
			test_case.future.wait();
		}

		for (auto& observer : observers) observer.access([&](auto& observer) {
			observer->test_suite_ended(test_suite);
		});
	}
}

unsigned tests_scope::new_test_suite(const string& description) {
	test_suites.emplace_back(test_suite{
		(unsigned)test_suites.size(),
		description,
		{}
	});
	auto& current_test_suite = test_suites.back();

	for (auto& observer : observers) observer.access([&](auto& observer) {
		observer->test_suite_begun(current_test_suite);
	});

	return current_test_suite.id;
}

unsigned tests_scope::new_test_case (const string &description, const function<void()>& execution_block) {
	auto& current_test_suite = test_suites.back();

	current_test_suite.test_cases.emplace_back(test_case{
		(unsigned)current_test_suite.test_cases.size(),
		description,
		chrono::high_resolution_clock::duration{0},
		test_case::status::running,
		"",
		std::future<void>{},
		current_test_suite
	});

	auto& current_test_case = current_test_suite.test_cases.back();

	for (auto& observer : observers) observer.access([&](auto& observer) {
		observer->test_case_begun(current_test_case);
	});

	current_test_case.future = tests_pool.exec([
		this,
		&current_test_case,
		execution_block
	]() {
		string low_level_error_message;

		stopwatch stopwatch;
		try {
			execution_block();

			successful_tests_count++;

			current_test_case.duration = stopwatch.total_time();
			current_test_case.status = test_case::status::successful;

			for (auto& observer : observers) observer.access([&](auto& observer) {
				observer->test_case_ended(current_test_case);
			});
		} catch (const exception &e) {
			failed_tests_count++;

			current_test_case.duration = stopwatch.total_time();
			current_test_case.status = test_case::status::failed;
			current_test_case.failure_reason = e.what();

			for (auto& observer : observers) observer.access([&](auto& observer) {
				observer->test_case_ended(current_test_case);
			});
		}
	});

	return current_test_case.id;
}
