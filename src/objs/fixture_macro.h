#pragma once

#include "label_macro.h"
#include "tests_scope.h"
#include "fixture.h"

#include <memory>
#include <functional>

#define setup(fixture_type, fixture_label)\
	ASSERT_LABEL_NOT_DEFINED(assert_setup_method, "cannot define multiple setups in a single test_suite");\
	ASSERT_LABEL_DEFINED(assert_test_case_block, "cannot declare setup outside test_suite");\
	std::function<fixture_type()> assert_setup_method;\
	std::shared_ptr<test::fixture<fixture_type>> assert_fixture(new test::fixture<fixture_type>);\
	auto& fixture_label = assert_fixture;\
	goto GENERATE_LABEL(ASSERT_LABEL_BEGIN_SETUP_BLOCK);\
	while(true)\
		if (true) {\
			fixture_label->set_setup(assert_setup_method);\
			break;\
		} else\
			GENERATE_LABEL(ASSERT_LABEL_BEGIN_SETUP_BLOCK):\
				assert_setup_method = []()

#define teardown(fixture_label) ;\
	ASSERT_LABEL_NOT_DEFINED(assert_teardown_method, "cannot define multiple teardowns in a single test_suite");\
	ASSERT_LABEL_DEFINED(assert_setup_method, "cannot declare teardown without a setup");\
	std::function<void(decltype(assert_fixture)::element_type::underlying_type&)> assert_teardown_method;\
	goto GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEARDOWN_BLOCK);\
	while(true)\
		if(true) {\
			assert_fixture->set_teardown(assert_teardown_method);\
			break;\
		} else\
			GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEARDOWN_BLOCK):\
				assert_teardown_method = [](decltype(assert_fixture)::element_type::underlying_type& fixture_label)

