#pragma once

#include <list>
#include <memory>
#include <atomic>
#include <parallel-tools/complex_atomic.h>
#include <parallel-tools/thread_pool.h>

#include "observers/observer.h"
#include "test_suite.h"
#include "test_case.h"

namespace test {
	class tests_scope {
		private:
			std::list<test_suite> test_suites;

			parallel_tools::thread_pool tests_pool;
			std::list<parallel_tools::complex_atomic<std::unique_ptr<observer>>> observers;
			std::atomic<unsigned> successful_tests_count;
			std::atomic<unsigned> failed_tests_count;
		public:
			tests_scope();
			~tests_scope();

			void wait_tests();

			unsigned new_test_suite(const std::string& description);
			unsigned new_test_case(const std::string& description, const std::function<void(void)>& execution_block);


	};
}

