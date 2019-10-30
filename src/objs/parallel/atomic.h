#pragma once

#include <memory>
#include <mutex>

namespace parallel {

	template<typename T>
	class atomic_access {
		private:
			std::lock_guard<std::mutex> lock;
			T& object;
		public:
			atomic_access(T& object, std::mutex &mutex)
				:	lock(mutex),
					object(object)
			{}

			T& operator* (void) {
				return this->object;
			}
	};

	template<typename T>
	class atomic {
		private:
			std::unique_ptr<std::mutex> sync_mutex;
			T object;

		public:
			atomic(void)
				:	sync_mutex(new std::mutex),
					object()
			{}

			atomic(T object)
				:	sync_mutex(new std::mutex),
				   	object(object)
			{}

			atomic_access<T> operator* (void) {
				return atomic_access<T>(this->object, *this->sync_mutex);
			}
	};

}
