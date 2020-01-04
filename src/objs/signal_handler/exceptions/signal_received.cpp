#include "signal_received.h"

using namespace test;
using namespace std;

signal_received::signal_received(int signal, const std::string& message) :
	signal(signal),
	message(message)
{}

const char* signal_received::what() const noexcept {
	return message.c_str();
}

