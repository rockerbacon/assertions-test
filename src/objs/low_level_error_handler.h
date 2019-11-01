#pragma once

#include <thread>
#include <csetjmp>

namespace test {
	void setup_signal_handlers (std::string* error_message, jmp_buf* jump_buffer);
}

