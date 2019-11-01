#pragma once

#include <string>
#include <chrono>

namespace test {

	class observer {
		public:
			virtual ~observer (void) = default;

			virtual void tests_begun (void) = 0;
			virtual void test_suite_block_begun (const std::string& test_suite_description) = 0;
			virtual void test_case_discovered (const std::string& test_case_description) = 0;
			virtual void test_suite_block_ended (void) = 0;
			virtual void test_case_execution_begun (const std::string& test_case_description, unsigned row) = 0;
			virtual void test_case_failed (const std::string& test_case_description, unsigned row, std::chrono::high_resolution_clock::duration test_duration, const std::string& reason) = 0;
			virtual void test_case_succeeded (const std::string& test_case_description, unsigned row, std::chrono::high_resolution_clock::duration test_duration) = 0;
			virtual void tests_ended (unsigned successful_tests, unsigned failed_tests) = 0;
	};


}
