#pragma once

#include "observer.h"

namespace test {

	class json_logger : public observer {
		private:
			std::ostream& out_stream;
		public:
			json_logger(std::ostream& out_stream);

			void tests_ended (unsigned failed_tests, unsigned successful_tests);
			void test_suite_begun (const test_suite& test_suite);
			void test_suite_ended (const test_suite& test_suite);
			void test_case_begun (const test_case& test_case);
			void test_case_ended (const test_case& test_case);
	};

}
