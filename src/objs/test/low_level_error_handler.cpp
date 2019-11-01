#include "low_level_error_handler.h"
#include <unordered_map>
#include <signal.h>
#include <string.h>

using namespace std;
using namespace test;

struct test_context {
	string* error_message;
	jmp_buf* jump_buffer;
};

unordered_map<thread::id, test_context> test_execution_context;

void signal_handler (int signal) {
	auto context = test_execution_context[this_thread::get_id()];

	switch (signal) {
		case SIGSEGV:
			*context.error_message = "segmentation fault";
			longjmp(*context.jump_buffer, 1);
			break;
	}
}

void test::setup_signal_handlers (string* error_message, jmp_buf* jump_buffer) {
	test_execution_context[this_thread::get_id()] = {
		error_message,
		jump_buffer
	};
	signal(SIGSEGV, signal_handler);
}

