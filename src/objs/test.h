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
#include "utils/warnings.h"
#include "fixture.h"

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

#define test_suite(test_suite_description)\
	ASSERT_LABEL_DEFINED(assert_test_suite_block, "cannot declare test_suite outside begin_tests");\
	::test::notify_test_suite_block_begun(test_suite_description);\
	::test::elements_discovered++;\
	bool ASSERT_GENERATE_LABEL(assert_test_suite_running) = true;\
	for (::test::test_case assert_test_case_block; ASSERT_GENERATE_LABEL(assert_test_suite_running);)\
		if (true)\
			goto ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_SUITE_BLOCK);\
		else\
			while(ASSERT_GENERATE_LABEL(assert_test_suite_running))\
				if (true) {\
					::test::notify_test_suite_block_ended();\
					ASSERT_GENERATE_LABEL(assert_test_suite_running) = false;\
				} else\
					ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_SUITE_BLOCK):

#define test_case(test_case_description)\
	ASSERT_LABEL_DEFINED(assert_test_case_block, "cannot declare test_case outside test_suite");\
	goto ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEST_CASE_BLOCK);\
	while(true)\
		if (true) {\
			::test::notify_test_case_discovered(test_case_description);\
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
		error_message << "expected " << #actual_value << ' ' << #comparison_operator << ' ' << expected_value;\
		error_message << " but got " << actual_value;\
		throw ::test::assert_failed(error_message.str());\
	}

#define begin_tests\
	int main (void) {\
		[[maybe_unused]] int assert_test_suite_block;\
		::test::observers.emplace_back(new ::test::live_terminal);\
		::test::observers.emplace_back(new ::test::json_logger(::std::cerr));\
		::test::notify_tests_begun();

#define end_tests\
		::test::wait_tests();\
		::test::notify_tests_ended(::test::successful_tests_count, ::test::failed_tests_count);\
		return ::test::failed_tests_count;\
	}

#define setup(fixture_type, fixture_label)\
	ASSERT_LABEL_NOT_DEFINED(assert_setup_method, "cannot define multiple setups in a single test_suite");\
	ASSERT_LABEL_DEFINED(assert_test_case_block, "cannot declare setup outside test_suite");\
	::std::function<fixture_type()> assert_setup_method;\
	::test::fixture<::std::function<fixture_type()>> assert_fixture;\
	auto& fixture_label = assert_fixture;\
	goto ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_SETUP_BLOCK);\
	while(true)\
		if (true) {\
			assert_fixture = ::test::fixture<::std::function<fixture_type()>>(assert_setup_method);\
			break;\
		} else\
			ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_SETUP_BLOCK):\
				assert_setup_method = []()

#define teardown(fixture_label) ;\
	ASSERT_LABEL_NOT_DEFINED(assert_teardown_method, "cannot define multiple teardowns in a single test_suite");\
	ASSERT_LABEL_DEFINED(assert_setup_method, "cannot declare teardown without a setup");\
	decltype(assert_fixture)::teardown_function assert_teardown_method;\
	goto ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEARDOWN_BLOCK);\
	while(true)\
		if(true) {\
			assert_fixture.set_teardown(assert_teardown_method);\
			break;\
		} else\
			ASSERT_GENERATE_LABEL(ASSERT_LABEL_BEGIN_TEARDOWN_BLOCK):\
				assert_teardown_method = [](decltype(assert_fixture)::fixture_type& fixture_label)


namespace test {

	extern std::list<parallel_tools::complex_atomic<std::unique_ptr<observer>>> observers;

	extern unsigned elements_discovered;

	extern std::atomic<unsigned> successful_tests_count;
	extern std::atomic<unsigned> failed_tests_count;

	extern parallel_tools::thread_pool tests_pool;
	extern std::list<std::future<void>> tests_futures;

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

	void notify_tests_begun();
	void notify_test_suite_block_begun(const std::string& test_suite_description);
	void notify_test_case_discovered(const std::string& test_case_description);
	void notify_test_suite_block_ended();
	void notify_test_case_execution_begun(const std::string& test_case_description, unsigned row);
	void notify_test_case_failed(
		const std::string& test_case_description,
		unsigned row,
		std::chrono::high_resolution_clock::duration test_duration,
		const std::string& reason
	);
	void notify_test_case_succeeded(
		const std::string& test_case_description,
		unsigned row,
		std::chrono::high_resolution_clock::duration test_duration
	);
	void notify_tests_ended (unsigned successful_tests, unsigned failed_tests);

	void wait_tests();

	// dummy for checking test_suite scope
	void assert_test_case_block(void);

	// dummy for checking begin_tests scope
	void assert_test_suite_block(void);

	//dummy for checking setup redeclaration
	void assert_setup_method(void);

	//dummy for checking teardown declaration
	void assert_teardown_method(void);

}

