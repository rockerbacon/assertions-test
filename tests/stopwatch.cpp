#include <test/assert.h>
#include <benchmark/stopwatch.h>

using namespace benchmark;

begin_tests {

	test_suite("stopwatch") {
		test_case("stopwatch constructor raises no errors") {
			Stopwatch stopwatch;
		};

		test_case("reseting stopwatch raises no errors") {
			Stopwatch stopwatch;
			stopwatch.reset();
		};

		test_case("capturing total time returns value greater than 0") {
			Stopwatch stopwatch;
			auto totalTime = stopwatch.totalTime();
			assert(totalTime.count(), >, 0);
		};

		test_case("capturing lap time returns value greater than 0") {
			Stopwatch stopwatch;
			auto lapTime = stopwatch.lapTime();
			assert(lapTime.count(), >, 0);
		};
	}

} end_tests;
