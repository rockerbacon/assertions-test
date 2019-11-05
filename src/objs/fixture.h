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
			std::mutex& mutex;
			mutable bool initialized;
			function_type setup;
			std::function<void(fixture_type&)> teardown;
			fixture_type &fixture_singleton;

		public:
			test_context(std::mutex& mutex, fixture_type& fixture_singleton)
				:	mutex(mutex),
					initialized(false),
					fixture_singleton(fixture_singleton)
			{}

			test_context& operator=(const function_type &function) {
				this->setup = function;
				return *this;
			}

			fixture_type mutable_fixture() const {
				return this->setup();
			}

			operator const fixture_type& () const {
				if (!this->initialized) {
					std::lock_guard<std::mutex> lock(this->mutex);
					if (!this->initialized) {
						this->fixture_singleton = this->setup();
						this->initialized = true;
					}
				}
				return this->fixture_singleton;
			}
	};

}

namespace std {
	template<typename function_type>
	typename test::test_context<function_type>::fixture_type copy(const test::test_context<function_type>& context) {
		return context.mutable_fixture();
	}
}


