#pragma once

#include <functional>
#include <parallel-tools/thread_pool.h>
#include <parallel-tools/complex_atomic.h>

#include "tests_scope.h"
#include "test_suite_macro.h"
#include "fixture_macro.h"
#include "test_case_macro.h"
#include "assert_macro.h"

#define tests\
	test::tests_scope assert_tests_scope;\
	int main ([[maybe_unused]] int assert_argc, [[maybe_unused]] char** assert_argv)

