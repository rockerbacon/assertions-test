#pragma once

#include <functional>
#include <type_traits>
#include <list>
#include <thread>
#include <sstream>
#include <iostream>
#include "observers/live_terminal.h"
#include "observers/json_logger.h"
#include "parallel/atomic.h"
#include "parallel/execution_queue.h"
#include "utils/warnings.h"
#include "fixture.h"

#define ASSERT_GENERATE_LABEL_PASTE_EXPAND(labelid, line) labelid ## _ ## line
#define ASSERT_GENERATE_LABEL_PASTE(labelid, line) ASSERT_GENERATE_LABEL_PASTE_EXPAND(labelid, line)
#define ASSERT_GENERATE_LABEL(labelid) ASSERT_GENERATE_LABEL_PASTE(labelid, __LINE__)

#define ASSERT_LABEL_NOT_DEFINED(label, error_message)\
	{\
		using namespace test;\
		static_assert(!::std::is_same<decltype(label), decltype(::test::label)>(), error_message);\
	}

#define test_suite(test_suite_description)\
	ASSERT_LABEL_NOT_DEFINED(assert_test_suite_block, "cannot declare test_suite outside begin_tests");\
	for (auto& observer : ::test::observers) {\
		(**observer)->test_suite_block_begun(test_suite_description);\
	}\
	::test::elements_discovered++;\
	if(false) {\
		ASSERT_GENERATE_LABEL(ASSERT_LABEL_END_TEST_SUITE_BLOCK):;\
			for (auto& observer : ::test::observers) {\
				(**observer)->test_suite_block_ended();\
			}\
	} else\
		for (::test::test_case assert_test_case_block;;)\
			if (true)\
				goto ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_SUITE_BLOCK);\
			else\
				while(true)\
					if (true) {\
						goto ASSERT_GENERATE_LABEL(ASSERT_LABEL_END_TEST_SUITE_BLOCK);\
					} else\
						ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_SUITE_BLOCK):

#define test_case(test_case_description)\
	ASSERT_LABEL_NOT_DEFINED(assert_test_case_block, "cannot declare test_case outside test_suite");\
	goto ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_CASE_BLOCK);\
	while(true)\
		if (true) {\
			for (auto& observer : ::test::observers) {\
				(**observer)->test_case_discovered(test_case_description);\
			}\
			::test::queue_test_for_execution(test_case_description, ::test::elements_discovered, assert_test_case_block);\
			::test::elements_discovered++;\
			break;\
		} else\
			ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_CASE_BLOCK):\
				assert_test_case_block = [=]()

#define assert(actual_value, comparison_operator, expected_value)\
	::test::is_logic_operator([=]{ return #comparison_operator; });\
	if (!((actual_value) comparison_operator (expected_value))) {\
		::std::stringstream error_message;\
		error_message << "expected " << #actual_value << ' ' << #comparison_operator << ' ' << #expected_value;\
		throw ::test::assert_failed(error_message.str());\
	}

#define begin_tests\
	int main (void) {\
		NO_UNUSED_WARNING int assert_test_suite_block;\
		::test::observers.emplace_back(new ::test::live_terminal);\
		::test::observers.emplace_back(new ::test::json_logger(::std::cerr));\
		for (auto& observer : ::test::observers) {\
			(**observer)->tests_begun();\
		}\

#define end_tests\
		::test::test_execution_queue.join_unfinished_executions();\
		for (auto& observer : ::test::observers) {\
			auto observer_access = *observer;\
			(*observer_access)->tests_ended(**::test::successful_tests_count, **::test::failed_tests_count);\
			delete *observer_access;\
		}\
		return **::test::failed_tests_count;\
	}

#define setup(fixture_type, fixture_label)\
	::std::mutex assert_setup_mutex;\
	fixture_type assert_setup_fixture;\
	::test::test_context<::std::function<fixture_type()>> fixture_label(assert_setup_mutex, assert_setup_fixture);\
	fixture_label = []()

namespace test {

	extern std::list<parallel::atomic<observer*>> observers;

	extern unsigned elements_discovered;

	extern parallel::atomic<unsigned> successful_tests_count;
	extern parallel::atomic<unsigned> failed_tests_count;

	extern parallel::execution_queue test_execution_queue;

	typedef std::function<void(void)> test_case;

	class assert_failed : public std::exception {
		private:
			const std::string message;
		public:
			assert_failed(const std::string &message);

			const char* what(void) const noexcept;
	};

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

	void queue_test_for_execution (const std::string &test_case_description, unsigned row_in_terminal, const test_case& test);

	// dummy for checking test_suite scope
	void assert_test_case_block(void);

	// dummy for checking begin_tests scope
	void assert_test_suite_block(void);

}

