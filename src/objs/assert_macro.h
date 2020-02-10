#pragma once

#include "exceptions/assert_failed.h"

#include <sstream>

#define assert(actual_value, comparison_operator, expected_value)\
	assert_macro::is_logic_operator([=]{ return #comparison_operator; });\
	if (!((actual_value) comparison_operator (expected_value))) {\
		using namespace std;\
		stringstream error_message;\
		error_message\
			<< "expected " << #actual_value << ' ' << #comparison_operator << ' ' << expected_value\
			<< " but got " << actual_value\
		;\
		throw test::assert_failed(error_message.str());\
	}

namespace assert_macro {
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
}

