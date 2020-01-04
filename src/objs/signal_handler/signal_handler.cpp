#include "signal_handler.h"
#include "exceptions/segmentation_fault.h"
#include "exceptions/abort_requested.h"
#include <functional>

using namespace test;
using namespace std;

void sigsegv_handler ([[maybe_unused]] int signal) {
	throw segmentation_fault();
}

void sigabrt_handler ([[maybe_unused]] int signal) {
	throw abort_requested();
}

void generic_handler(int signal_code) {
	throw signal_received(signal_code, "received signal "s + to_string(signal_code));
}

void test::handle_signal (int signal_code) {
	function<void(int)> signal_handler;
	switch (signal_code) {
		case SIGSEGV:
			signal_handler = sigsegv_handler;
			break;
		case SIGABRT:
			signal_handler = sigabrt_handler;
			break;
		default:
			signal_handler = generic_handler;
	}
	signal(signal_code, *signal_handler.target<void(*)(int)>());
}

