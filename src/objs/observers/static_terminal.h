#pragma once

#include "observer.h"

#include <sstream>
#include <unordered_map>

namespace test {
	class static_terminal : public observer {
		private:
			std::ostream& out_stream;
		public:
			static_terminal();

			void tests_begun();
			void tests_ended(unsigned failed_tests, unsigned successful_tests);
			void test_suite_begun (const test_suite& test_suite);
			void test_suite_ended (const test_suite& test_suite);
			void test_case_begun (const test_case& test_case);
			void test_case_ended (const test_case& test_case);
	};
}

