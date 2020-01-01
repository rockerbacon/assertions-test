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

			operator T& () {
				return this->object;
			}

			T& operator-> () {
				return this->object;
			}
	};

	template<typename T>
	class atomic {
		private:
			std::mutex sync_mutex;
			T object;

		public:
			atomic(T object) :
				object(object)
			{}

			atomic_access<T> operator* (void) {
				return atomic_access<T>(this->object, this->sync_mutex);
			}

			atomic_access<T> operator-> () {
				return atomic_access<T>(this->object, this->sync_mutex);
			}
	};

}
