#include <test.h>
#include <thread>
#include <chrono>

using namespace std;
using namespace chrono;

tests {

	test_suite("segmentation fault") {
		test_case ("do something before low level error executes") {
			assert(true, ==, true);
		};

		test_case("raises low level error after 2s") {
			int* x = NULL;
			this_thread::sleep_for(2s);
			*x = 2;
			assert(*x, ==, 2);
		};

		test_case("continue execution normally") {
			std::this_thread::sleep_for(3s);
		};
	}

}

