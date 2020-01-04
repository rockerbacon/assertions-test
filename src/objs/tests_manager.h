#pragma once

#include <list>
#include <memory>
#include <atomic>
#include <future>
#include <parallel-tools/complex_atomic.h>
#include <parallel-tools/thread_pool.h>

#include "observers/observer.h"

namespace test {
	class tests_manager {
		private:
			parallel_tools::thread_pool tests_pool;
			std::list<parallel_tools::complex_atomic<std::unique_ptr<observer>>> observers;
			std::list<std::future<void>> tests_futures;
			std::atomic<unsigned> successful_tests_count;
			std::atomic<unsigned> failed_tests_count;
		public:
			tests_manager();
			~tests_manager();

			void queue_test_for_execution(const std::string& test_case_description, unsigned row_in_terminal, const std::function<void(void)>& test_case);

			void notify_tests_begun();
			void notify_test_suite_block_begun(const std::string& test_suite_description);
			void notify_test_case_discovered(const std::string& test_case_description);
			void notify_test_suite_block_ended();
			void notify_test_case_execution_begun(const std::string& test_case_description, unsigned row);
			void notify_test_case_failed(
				const std::string& test_case_description,
				unsigned row,
				std::chrono::high_resolution_clock::duration test_duration,
				const std::string& reason
			);
			void notify_test_case_succeeded(
				const std::string& test_case_description,
				unsigned row,
				std::chrono::high_resolution_clock::duration test_duration
			);
			void notify_tests_ended (unsigned successful_tests, unsigned failed_tests);
			void wait_tests();


	};
}

