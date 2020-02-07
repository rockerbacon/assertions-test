#pragma once

#include <mutex>
#include <type_traits>
#include <functional>
#include <atomic>
#include <memory>

namespace test {

	template<typename fixture_type>
	class fixture {
		public:
			typedef fixture_type underlying_type;
		private:
			mutable std::mutex mutex;
			mutable std::atomic<bool> initialized;
			std::function<fixture_type()> setup;
			std::function<void(fixture_type&)> teardown;
			mutable std::unique_ptr<fixture_type> fixture_singleton;

		public:
			fixture() :
				initialized(false),
				teardown([](fixture_type&){})
			{}

			explicit fixture(const fixture& other) :
				fixture()
			{
				set_setup(other.setup);
				set_teardown(other.teardown);
			}

			fixture(fixture&& other) :
				initialized(other.initialized.load()),
				setup(other.setup),
				teardown(other.teardown),
				fixture_singleton(std::move(other.fixture_singleton))
			{}

			~fixture() {
				if (initialized.load()) {
					teardown(*fixture_singleton);
				}
			}

			operator const fixture_type& () const {
				if (!initialized.load()) {
					std::lock_guard<std::mutex> lock(mutex);
					if (!initialized.load()) {
						fixture_singleton = std::make_unique<fixture_type>(setup());
						initialized.store(true);
					}
				}
				return *fixture_singleton;
			}

			void set_setup(const decltype(setup)& setup) {
				this->setup = setup;
			}

			void set_teardown(const decltype(teardown)& teardown) {
				this->teardown = teardown;
			}
	};

	template<typename fixture_type>
	fixture<fixture_type> copy(const fixture<fixture_type>& original) {
		return fixture<fixture_type>(original);
	}

}

