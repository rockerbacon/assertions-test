#pragma once

#include <mutex>
#include <type_traits>
#include <functional>
#include <memory>

namespace test {

	template<typename function_type>
	class fixture {
		template<typename T>
		friend fixture<T> std::copy(const fixture<T>&);
		public:
			typedef decltype(std::declval<function_type>()()) fixture_type;
		private:
			std::shared_ptr<std::mutex> mutex;
			mutable bool initialized;
			std::shared_ptr<function_type> setup;
			std::shared_ptr<std::function<void()>> teardown;
			mutable std::shared_ptr<fixture_type> fixture_singleton;

		public:
			fixture(decltype(setup) setup, decltype(teardown) teardown)
				:	mutex(new std::mutex),
					initialized(false),
					setup(setup),
					teardown(teardown)
			{}

			operator fixture_type () const {
				if (!this->initialized) {
					std::lock_guard<std::mutex> lock(*this->mutex);
					if (!this->initialized) {
						this->fixture_singleton = std::make_shared<fixture_type>((*this->setup)());
						this->initialized = true;
					}
				}
				return *this->fixture_singleton;
			}
	};

}

namespace std {
	template<typename function_type>
	typename test::fixture<function_type> copy(const test::fixture<function_type>& original) {
		test::fixture<function_type> copy(original.setup, original.teardown);
		return copy;
	}
}


