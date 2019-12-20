#pragma once

#include <mutex>
#include <type_traits>
#include <functional>
#include <memory>

namespace test {

	template<typename setup_function>
	class fixture {
		public:
			typedef decltype(std::declval<setup_function>()()) fixture_type;
			typedef std::function<void(fixture_type&)> teardown_function;
		private:
			std::shared_ptr<std::mutex> mutex;
			mutable bool initialized;
			setup_function setup;
			teardown_function teardown;
			mutable std::shared_ptr<fixture_type> fixture_singleton;

		public:
			fixture() = default;
			fixture(decltype(setup) setup)
				:	mutex(new std::mutex),
					initialized(false),
					setup(setup),
					teardown([](fixture_type&){})
			{}

			fixture(const fixture& other)
				:	fixture(other.setup)
			{
				this->set_teardown(other.teardown);
			}

			~fixture() {
				if (this->initialized && this->fixture_singleton.use_count() == 1) {
					this->teardown(*this->fixture_singleton);
				}
			}

			operator fixture_type () const {
				if (!this->initialized) {
					std::lock_guard<std::mutex> lock(*this->mutex);
					if (!this->initialized) {
						this->fixture_singleton = std::make_shared<fixture_type>(this->setup());
						this->initialized = true;
					}
				}
				return *this->fixture_singleton;
			}

			void set_teardown(decltype(teardown) teardown) {
				this->teardown = teardown;
			}
	};

}

namespace std {
	template<typename function_type>
	typename test::fixture<function_type> copy(const test::fixture<function_type>& original) {
		return test::fixture<function_type>(original);
	}
}

