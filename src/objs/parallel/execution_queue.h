#pragma once

#include <mutex>
#include <condition_variable>
#include <thread>
#include <list>
#include <functional>
#include <parallel/atomic.h>

namespace parallel {

	struct executing_thread {
		std::thread::native_handle_type native_handle;
		bool is_running;
	};

	class execution_queue {
		private:
			std::unordered_map<std::thread::id, executing_thread> threads;
			std::mutex mutex;
			std::condition_variable notifier;
			const unsigned max_queue_size;
			unsigned available_threads;
			unsigned queued_executions;
		public:
			execution_queue(unsigned queue_size);

			void push_back(const std::function<void(void)>& execution);
			void join_unfinished_executions (void);

			void terminate (std::thread::id thread_id);
	};

}
