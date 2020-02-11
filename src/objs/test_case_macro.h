#pragma once

#include "label_macro.h"
#include "tests_scope.h"

#define test_case(test_case_description)\
	ASSERT_LABEL_DEFINED(assert_test_case_block, "cannot declare test_case outside test_suite");\
	goto GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_CASE_BLOCK);\
	while(true)\
		if (true) {\
			assert_tests_scope.new_test_case(test_case_description, assert_test_case_block);\
			break;\
		} else\
			GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_CASE_BLOCK):\
				assert_test_case_block = [=]()

