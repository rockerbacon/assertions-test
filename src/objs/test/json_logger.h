#pragma once

#include "observer.h"

namespace test {

	class json_logger : public observer {
		private:
			std::ostream& out_stream;
		public:
			json_logger(std::ostream& out_stream);

			void tests_begun (void);
			void test_suite_block_begun (const std::string& test_suite_description);
			void test_case_discovered (const std::string& test_case_description);
			void test_suite_block_ended (void);
			void test_case_execution_begun (const std::string& test_case_description, unsigned row);
			void test_case_failed (const std::string& test_case_description, unsigned row, std::chrono::high_resolution_clock::duration test_duration, const std::string& reason);
			void test_case_succeeded (const std::string& test_case_description, unsigned row, std::chrono::high_resolution_clock::duration test_duration);
			void tests_ended (unsigned successful_tests, unsigned failed_tests);
	};

}
