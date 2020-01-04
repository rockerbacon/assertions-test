#include "signal_handler.h"
#include "exceptions/segmentation_fault.h"

using namespace test;
using namespace std;

void sigsegv_handler ([[maybe_unused]] int signal) {
	throw segmentation_fault();
}

void generic_handler(int signal_code) {
	throw signal_received(signal_code, "received signal "s + to_string(signal_code));
}

void test::handle_signal (int signal_code) {
	switch (signal_code) {
		case SIGSEGV:
			signal(SIGSEGV, sigsegv_handler);
			break;
		default:
			signal(signal_code, generic_handler);
	}
}

