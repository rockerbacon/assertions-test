#include <test.h>

using namespace std::chrono_literals;

tests {
	test_suite("when using fixtures in test cases") {
		setup (int*, my_int_fixture) {
			int *processed_value = new int(10);
			return processed_value;
		};

		teardown (my_int_fixture) {
			delete my_int_fixture;
		};

		test_case("fixtures behave as singletons when captured as constant references") {
			const int* fixtureA = my_int_fixture;
			const int* fixtureB = my_int_fixture;

			assert(fixtureA, ==, fixtureB);
		};

		test_case("fixtures can be copied") {
			// warning: each new copy implies new executions of setup and teardown
			auto fixtureA = std::copy(my_int_fixture);
			auto fixtureB = std::copy(my_int_fixture);
			const int* non_copied_fixture = my_int_fixture;

			assert(fixtureA, !=, fixtureB);

			*fixtureA = 11;
			assert(*fixtureA, ==, 11);
			assert(*fixtureB, ==, 10);
			assert(*non_copied_fixture, ==, 10);
		};
	}

	test_suite("when tearing down fixtures") {
		setup (std::string, fixture) {
			return "this fixture was setup";
		};

		teardown (fixture) {
			fixture = "this fixture was torn down";
			std::cerr << (std::string)fixture << std::endl;
		};

		test_case("fixtures are not deleted before all tests in the test suite finish") {
			std::this_thread::sleep_for(1s);
			assert((std::string)fixture, ==, "this fixture was setup");
		};

		test_case("fixtures are correctly setup the first time") {
			assert((std::string)fixture, ==, "this fixture was setup");
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
}
