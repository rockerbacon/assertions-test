#include "segmentation_fault.h"
#include <csignal>

using namespace test;

segmentation_fault::segmentation_fault() :
	signal_received(SIGSEGV, "segmentation fault")
{}

