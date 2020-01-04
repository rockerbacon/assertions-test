#pragma once

#include <exception>
#include <string>

namespace test {
	class assert_failed : public std::exception {
		private:
			const std::string message;
		public:
			assert_failed(const std::string& message);

			const char* what() const noexcept;
	};
}

