#pragma once

#include <string>
#include <future>

namespace test {
	struct test_suite;

	struct test_case {
		enum status {
			successful,
			failed,
			running
		};

		unsigned id;
		std::string description;
		std::chrono::high_resolution_clock::duration duration;
		status status;
		std::string failure_reason;
		std::future<void> future;
		test::test_suite& test_suite;
	};
}

