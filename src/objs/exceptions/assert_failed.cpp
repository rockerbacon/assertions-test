#include "assert_failed.h"

using namespace test;
using namespace std;

assert_failed::assert_failed (const string& message) :
	message(message)
{}

const char* assert_failed::what(void) const noexcept {
	return message.c_str();
}

