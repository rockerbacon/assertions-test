#pragma once

#include "label_macro.h"
#include "tests_scope.h"

#define test_suite(test_suite_description)\
	ASSERT_LABEL_DEFINED(assert_argc, "cannot declare test_suite outside tests");\
	assert_tests_scope.new_test_suite(test_suite_description);\
	bool GENERATE_LABEL(parsing_test_suite) = true;\
	for (std::function<void()> assert_test_case_block; GENERATE_LABEL(parsing_test_suite);)\
		if (true)\
			goto GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_SUITE_BLOCK);\
		else\
			while(GENERATE_LABEL(parsing_test_suite))\
				if (true) {\
					GENERATE_LABEL(parsing_test_suite) = false;\
				} else\
					GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_SUITE_BLOCK):

