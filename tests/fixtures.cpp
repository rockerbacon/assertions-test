#include <test.h>

begin_tests {
	test_suite("when working with fixtures") {
		setup (int*, my_int_fixture) {
			int *processed_value = new int(10);
			return processed_value;
		} teardown {
			delete my_int_fixture;
		};

		test_case("fixtures behave as singletons when captured as constant references") {
			const int* fixtureA = my_int_fixture;
			const int* fixtureB = my_int_fixture;

			assert(fixtureA, ==, fixtureB);

			assert(*fixtureA, ==, 10);
			assert(*fixtureB, ==, 10);
		};

		test_case("fixtures can be copied") {
			// warning: each new copy implies new executions of setup and teardown
			auto fixtureA = std::copy(my_int_fixture);
			auto fixtureB = std::copy(my_int_fixture);

			assert(fixtureA, !=, fixtureB);

			*fixtureA = 11;
			assert(*fixtureA, ==, 11);
			assert(*fixtureB, ==, 10);
		};
	}

	test_suite("fixtures can be any user defined label") {
		setup (int, another_fixture) {
			int processed_value = 10;
			return processed_value;
		};

		test_case("fixtures behave as singletons when captured as constant references") {
			int fixtureA = another_fixture;

			assert(fixtureA, ==, 10);
		};
	}

	test_suite("fixtures can be used in non modifying expressions") {
		setup(double, three_fixture) {
			return 3.0;
		};

		test_case("can be used in arithmetic expressions") {
			auto six = three_fixture*2;
			assert(six, ==, 6.0);
		};

		test_case("can be used in comparisons") {
			assert(three_fixture, ==, 3.0);
		};
	}
} end_tests;
