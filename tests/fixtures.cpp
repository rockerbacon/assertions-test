#include <test.h>

begin_tests {
	test_suite("when working with fixtures") {
		setup (int*) {
			int *processed_value = new int;
			*processed_value = 10;
			return processed_value;
		};

		test_case("fixtures behave as singletons when used as constants") {
			const int* fixtureA = context.fixture();
			const int* fixtureB = context.fixture();
			assert(fixtureA, ==, fixtureB);
		};

		test_case("fixtures will be copied when they need to be modifiable") {
			// warning: the setup function will be executed each time fixture is requested as non const
			int* fixtureA = context.mutable_fixture();
			int* fixtureB = context.mutable_fixture();

			assert(fixtureA, !=, fixtureB);

			*fixtureA = 11;
			assert(*fixtureA, ==, 11);
			assert(*fixtureB, ==, 10);
		};
	}
} end_tests;
