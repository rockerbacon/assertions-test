#pragma once

#include "../test_suite.h"
#include "../test_case.h"

#include <string>
#include <chrono>


namespace test {
	class observer {
		public:
			virtual ~observer () = default;

			virtual void tests_begun () = 0;
			virtual void tests_ended (unsigned failed_tests, unsigned successful_tests) = 0;
			virtual void test_suite_begun (const test_suite& test_suite) = 0;
			virtual void test_suite_ended (const test_suite& test_suite) = 0;
			virtual void test_case_begun (const test_case& test_case) = 0;
			virtual void test_case_ended (const test_case& test_case) = 0;
	};
}
