#include <test/assert.h>

begin_tests {

	test_suite("test suite 1") {

		test_case("test case block") {
			NO_UNUSED_WARNING int a = 2;
		};

		test_case("multiple test case blocks") {
			NO_UNUSED_WARNING int b = 3;
		};

		test_case("even more test case blocks") {
			NO_UNUSED_WARNING float c = 5.4;
		};

	}

	test_suite("multiple test suites") {
		test_case("trying something") {
			NO_UNUSED_WARNING float pi = 3.14;
		};

		test_case("trying something else") {
			NO_UNUSED_WARNING float pi2 = 3.1415;
		};
	}

	test_suite("empty test suite") {

	}

} end_tests;
