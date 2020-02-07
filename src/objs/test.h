#pragma once

#include <functional>
#include <type_traits>
#include <list>
#include <thread>
#include <sstream>
#include <iostream>
#include <parallel-tools/thread_pool.h>
#include <parallel-tools/complex_atomic.h>

#include "observers/live_terminal.h"
#include "observers/json_logger.h"
#include "fixture.h"
#include "tests_manager.h"
#include "exceptions/assert_failed.h"

#define ASSERT_GENERATE_LABEL_PASTE_EXPAND(labelid, line) labelid ## _ ## line
#define ASSERT_GENERATE_LABEL_PASTE(labelid, line) ASSERT_GENERATE_LABEL_PASTE_EXPAND(labelid, line)
#define ASSERT_GENERATE_LABEL(labelid) ASSERT_GENERATE_LABEL_PASTE(labelid, __LINE__)

#define ASSERT_LABEL_DEFINED(label, error_message)\
	{\
		using namespace test;\
		static_assert(!::std::is_same<decltype(label), decltype(::test::label)>(), error_message);\
	}

#define ASSERT_LABEL_NOT_DEFINED(label, error_message)\
	{\
		using namespace test;\
		static_assert(::std::is_same<decltype(label), decltype(::test::label)>(), error_message);\
	}

#define tests\
	::test::tests_manager assert_tests_manager;\
	int main ([[maybe_unused]] int assert_argc, [[maybe_unused]] char** assert_argv)

#define test_suite(test_suite_description)\
	ASSERT_LABEL_DEFINED(assert_argc, "cannot declare test_suite outside begin_tests");\
	assert_tests_manager.notify_test_suite_block_begun(test_suite_description);\
	::test::elements_discovered++;\
	bool ASSERT_GENERATE_LABEL(assert_test_suite_running) = true;\
	for (::std::function<void()> assert_test_case_block; ASSERT_GENERATE_LABEL(assert_test_suite_running);)\
		if (true)\
			goto ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_SUITE_BLOCK);\
		else\
			while(ASSERT_GENERATE_LABEL(assert_test_suite_running))\
				if (true) {\
					assert_tests_manager.notify_test_suite_block_ended();\
					ASSERT_GENERATE_LABEL(assert_test_suite_running) = false;\
				} else\
					ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_SUITE_BLOCK):

#define test_case(test_case_description)\
	ASSERT_LABEL_DEFINED(assert_test_case_block, "cannot declare test_case outside test_suite");\
	goto ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_CASE_BLOCK);\
	while(true)\
		if (true) {\
			assert_tests_manager.notify_test_case_discovered(test_case_description);\
			assert_tests_manager.queue_test_for_execution(test_case_description, ::test::elements_discovered, assert_test_case_block);\
			::test::elements_discovered++;\
			break;\
		} else\
			ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_CASE_BLOCK):\
				assert_test_case_block = [=]()

#define assert(actual_value, comparison_operator, expected_value)\
	::test::is_logic_operator([=]{ return #comparison_operator; });\
	if (!((actual_value) comparison_operator (expected_value))) {\
		::std::stringstream error_message;\
		error_message << "expected " << #actual_value << ' ' << #comparison_operator << ' ' << expected_value;\
		error_message << " but got " << actual_value;\
		throw ::test::assert_failed(error_message.str());\
	}

#define setup(fixture_type, fixture_label)\
	ASSERT_LABEL_NOT_DEFINED(assert_setup_method, "cannot define multiple setups in a single test_suite");\
	ASSERT_LABEL_DEFINED(assert_test_case_block, "cannot declare setup outside test_suite");\
	::std::function<fixture_type()> assert_setup_method;\
	::std::shared_ptr<::test::fixture<fixture_type>> assert_fixture(new ::test::fixture<fixture_type>);\
	auto& fixture_label = assert_fixture;\
	goto ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_SETUP_BLOCK);\
	while(true)\
		if (true) {\
			fixture_label->set_setup(assert_setup_method);\
			break;\
		} else\
			ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_SETUP_BLOCK):\
				assert_setup_method = []()

#define teardown(fixture_label) ;\
	ASSERT_LABEL_NOT_DEFINED(assert_teardown_method, "cannot define multiple teardowns in a single test_suite");\
	ASSERT_LABEL_DEFINED(assert_setup_method, "cannot declare teardown without a setup");\
	::std::function<void(decltype(assert_fixture)::element_type::underlying_type&)> assert_teardown_method;\
	goto ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEARDOWN_BLOCK);\
	while(true)\
		if(true) {\
			assert_fixture->set_teardown(assert_teardown_method);\
			break;\
		} else\
			ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEARDOWN_BLOCK):\
				assert_teardown_method = [](decltype(assert_fixture)::element_type::underlying_type& fixture_label)


namespace test {

	extern unsigned elements_discovered;

	template<typename ConstexprStringFunction>
	constexpr void is_logic_operator(ConstexprStringFunction string_function) {
		static_assert(
			string_function()[0] == '=' ||
			string_function()[0] == '!' ||
			string_function()[0] == '<' ||
			string_function()[0] == '>'
			, "can only assert with logic operators"
		);
	}

	// dummy for checking test_suite scope
	void assert_test_case_block(void);

	// dummy for checking begin_tests scope
	void assert_argc(void);

	//dummy for checking setup redeclaration
	void assert_setup_method(void);

	//dummy for checking teardown declaration
	void assert_teardown_method(void);

}

