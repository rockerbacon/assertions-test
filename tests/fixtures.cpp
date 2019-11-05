#include <test.h>

begin_tests {
	test_suite("when working with fixtures") {
		setup (int, my_int_fixture) {
			int processed_value = 10;
			return processed_value;
		};

		test_case("fixtures behave as singletons when they are constant references") {
			const int& fixtureA = my_int_fixture;
			const int& fixtureB = my_int_fixture;

			assert(&fixtureA, ==, &fixtureB); // both point to the same memory address
		};

		test_case("fixtures must be copied when they need to be modifiable") {
			// warning: the setup function will be executed each time fixture is requested as non const
			int fixtureA = std::copy(my_int_fixture);
			int fixtureB = std::copy(my_int_fixture);

			assert(fixtureA, ==, fixtureB);

			fixtureA = 11;
			assert(fixtureA, ==, 11);
			assert(fixtureB, ==, 10);
		};
	}

	test_suite("fixtures can be any user defined label") {
		setup (int, another_fixture) {
			int processed_value = 10;
			return processed_value;
		};

		test_case("fixtures behave as singletons when used as constants") {
			const int& fixtureA = another_fixture;
			const int& fixtureB = another_fixture;

			assert(fixtureA, ==, fixtureB);
		};

		test_case("fixtures must be copied when they need to be modifiable") {
			// warning: the setup function will be executed each time fixture is requested as copy
			int fixtureA = std::copy(another_fixture);
			int fixtureB = std::copy(another_fixture);

			assert(fixtureA, ==, fixtureB);

			fixtureA = 11;
			assert(fixtureA, ==, 11);
			assert(fixtureB, ==, 10);
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
