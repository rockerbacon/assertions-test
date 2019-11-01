#include "execution_queue.h"

using namespace parallel;
using namespace std;

execution_queue::execution_queue (unsigned queue_size)
	:	max_queue_size(queue_size),
		available_threads(queue_size),
		queued_executions(0)
{}

void execution_queue::push_back(const std::function<void(void)>& execution) {
	lock_guard<std::mutex> lock(this->mutex);

	this->queued_executions++;
	thread queued_thread(([this, execution]() {
		{
			unique_lock<std::mutex> lock(this->mutex);

			this->notifier.wait(lock, [this]() {
				return this->available_threads > 0;
			});

			this->queued_executions--;
			this->available_threads--;
			this->threads[this_thread::get_id()].is_running = true;
		}

		execution();

		{
			unique_lock<std::mutex> lock(this->mutex);
			this->available_threads++;
			this->notifier.notify_one();
		}
	}));

	this->threads.emplace(queued_thread.get_id(), executing_thread{ queued_thread.native_handle(), false });
	queued_thread.detach();
}

void execution_queue::join_unfinished_executions (void) {
	unique_lock<std::mutex> lock(this->mutex);

	this->notifier.wait(lock, [this]() {
		return this->available_threads == this->max_queue_size && this->queued_executions == 0;
	});

}

void execution_queue::terminate (thread::id thread_id) {
	lock_guard<std::mutex> lock(this->mutex);

	auto thread_index = this->threads.find(thread_id);
	if (thread_index != this->threads.end()) {
#ifdef _WIN32
		// TODO non posix thread termination
#else
		pthread_cancel(thread_index->second.native_handle);
#endif
		if (thread_index->second.is_running) {
			this->available_threads++;
		}
		this->threads.erase(thread_id);
	}

}

