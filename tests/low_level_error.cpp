#include <test.h>
#include <future>
#include <chrono>
#include "signal_handler/exceptions/signal_received.h"

using namespace std;
using namespace chrono;

tests {
	test_suite("segmentation fault") {
		test_case ("do something before segmentation fault") {
			assert(true, ==, true);
		};

		test_case("segfaults in main thread after 500ms") {
			int* x = NULL;
			this_thread::sleep_for(500ms);
			*x = 2;
			assert(*x, ==, 2);
		};

		test_case("segfaults in async method after 750ms") {
			auto segfault_future = async(launch::async, [&] {
				int* x = NULL;
				this_thread::sleep_for(750ms);
				*x = 2;
				assert(*x, ==, 2);
			});

			segfault_future.get();
		};

		test_case("segfaults in thread after 1s") {
			bool exception_raised = false;
			std::thread([&] {
				try {
					int* x = NULL;
					this_thread::sleep_for(1s);
					*x = 2;
					assert(*x, ==, 2);
				} catch (const test::signal_received& e) {
					exception_raised = true;
				}
			}).join();

			assert(exception_raised, ==, true);
		};

		test_case("shuould continue execution normally after 1500ms") {
			std::this_thread::sleep_for(1500ms);
		};
	}
}

