#pragma once

#include <type_traits>

#define GENERATE_LABEL_PASTE_EXPAND(labelid, line) labelid ## _ ## line
#define GENERATE_LABEL_PASTE(labelid, line) GENERATE_LABEL_PASTE_EXPAND(labelid, line)
#define GENERATE_LABEL(labelid) GENERATE_LABEL_PASTE(labelid, __LINE__)

#define ASSERT_LABEL_DEFINED(label, error_message)\
	{\
		using namespace test_labels;\
		static_assert(!std::is_same<decltype(label), decltype(test_labels::label)>(), error_message);\
	}

#define ASSERT_LABEL_NOT_DEFINED(label, error_message)\
	{\
		using namespace test_labels;\
		static_assert(std::is_same<decltype(label), decltype(test_labels::label)>(), error_message);\
	}

namespace test_labels {
	void assert_test_case_block(void);
	void assert_argc(void);
	void assert_setup_method(void);
	void assert_teardown_method(void);
}

