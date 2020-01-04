#include <test.h>

tests {

	test_suite("test suite 1") {

		test_case("test case block") {
			[[maybe_unused]] int a = 2;
		};

		test_case("multiple test case blocks") {
			[[maybe_unused]] int b = 3;
		};

		test_case("even more test case blocks") {
			[[maybe_unused]] float c = 5.4;
		};

	}

	test_suite("multiple test suites") {
		test_case("trying something") {
			[[maybe_unused]] float pi = 3.14;
		};

		test_case("trying something else") {
			[[maybe_unused]] float pi2 = 3.1415;
		};
	}

	test_suite("empty test suite") {

	}

}
