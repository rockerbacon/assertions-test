#include <test.h>

begin_tests {

	test_suite("assert macros") {

		test_case("assert equal succesfully") {
			assert(3, ==, 3);
		};

		test_case("assert greater than succesfully") {
			assert(4, >, 2);
		};

		test_case("assert less than succesfully") {
			assert(2, <, 4);
		};

		test_case("assert grater than or equal when value is greater") {
			assert(4, >=, 2);
		};

		test_case("assert grater than or equal when values are equal") {
			assert(4, >=, 4);
		};

		test_case("fail assert equal") {
			assert(1, ==, 3);
		};

		test_case("multiple assertions in one test case - all successful") {
			assert(2, ==, 2);
			assert(true, ==, true);
			assert(2, <, 4);
		};

		test_case("multiple assertions in one test case - one fails") {
			assert(2, ==, 2);
			assert(true, ==, false);
			assert(2, <, 4);
		};
	}

} end_tests;
