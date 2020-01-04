#include <test.h>
#include <thread>

using namespace std;

tests {

	test_suite("4 test cases outputs") {
		test_case("wait 500ms") {
			this_thread::sleep_for(500ms);
		};

		test_case("finish immediately") {
			assert(true, ==, true);
		};

		test_case("wait 4s then fail") {
			this_thread::sleep_for(4s);
			assert(true, ==, false);
		};

		test_case("wait 3512ms") {
			this_thread::sleep_for(3512ms);
		};
	}

	test_suite("5 test cases outputs") {
		test_case("wait 5s") {
			this_thread::sleep_for(5s);
		};

		test_case("wait 2314ms then fail") {
			this_thread::sleep_for(2314ms);
			assert(true, ==, false);
		};

		test_case("wait 3s") {
			this_thread::sleep_for(3s);
		};

		test_case("wait 3300ms") {
			this_thread::sleep_for(3300ms);
		};

		test_case("wait 6120ms") {
			this_thread::sleep_for(6120ms);
		};
	}

	test_suite("2 test cases output") {
		test_case("wait 2500ms") {
			this_thread::sleep_for(2500ms);
		};

		test_case("wait 3s") {
			this_thread::sleep_for(3s);
		};
	}

}
