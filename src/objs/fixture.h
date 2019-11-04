#pragma once

#include <mutex>
#include <type_traits>
#include <functional>

namespace test {

	template<typename function_type>
	class test_context {
		public:
			typedef decltype(std::declval<function_type>()()) fixture_type;
		private:
			mutable std::mutex mutex;
			mutable bool initialized;
			function_type setup;
			std::function<void(fixture_type&)> teardown;
			mutable fixture_type fixture_singleton;


		public:
			test_context()
				:	initialized(false)
			{}

			test_context& operator=(const function_type &function) {
				this->setup = function;
				return *this;
			}

			const fixture_type& fixture() const {
				if (!this->initialized) {
					std::lock_guard<std::mutex> lock(this->mutex);
					if (!this->initialized) {
						this->fixture_singleton = this->setup();
						this->initialized = true;
					}
				}
				return this->fixture_singleton;
			}

			fixture_type mutable_fixture() const {
				return this->setup();
			}
	};
}

