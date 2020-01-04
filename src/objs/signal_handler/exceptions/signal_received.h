#pragma once

#include <exception>
#include <string>

namespace test {
	class signal_received : public std::exception {
		private:
			const int signal;
			const std::string message;
		public:
			signal_received (int signal, const std::string& message);
			const char* what() const noexcept;
	};
}

