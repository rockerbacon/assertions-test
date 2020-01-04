#include "abort_requested.h"

#include <csignal>

using namespace test;

abort_requested::abort_requested() :
	signal_received(SIGABRT, "abort (core dumped)")
{}

