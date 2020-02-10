#pragma once

#include <string>
#include <list>

#include "test_case.h"

namespace test {
	struct test_case;

	struct test_suite {
		unsigned id;
		std::string description;
		std::list<test_case> test_cases;
	};
}
