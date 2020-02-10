#include "static_terminal.h"
#include "../terminal/manipulators.h"
#include <stopwatch/stopwatch.h>

#include <iostream>
#include <utility>

using namespace std;
using namespace test;
using namespace terminal;

static_terminal::static_terminal() :
	out_stream(cout)
{}

void static_terminal::tests_ended(
	[[maybe_unused]] unsigned failed_tests,
	[[maybe_unused]] unsigned successful_tests
) {
	out_stream << endl;
}

void static_terminal::test_suite_begun(const test_suite& test_suite) {
	out_stream
	<< style<font::FAINT>
		<< "discovered test suite '" << test_suite.description << '\''
	<< style<RESET_STYLE> << endl;
}

void static_terminal::test_suite_ended (const test_suite& test_suite) {
	stringstream test_case_results;
	bool some_test_case_failed = false;

	test_case_results << '\n';
	for (auto& test_case : test_suite.test_cases) {
		if (test_case.status == test_case::status::failed) {
			some_test_case_failed = true;

			test_case_results
			<< style<bright<color::RED>()>
				<< ident(1) << icon::CROSS << "  " << test_case.description
				<< " (" << test_case.duration << "):\n"
					<< style<RESET_STYLE> << style<font::FAINT>
						<< ident(2) << test_case.failure_reason
					<< style<RESET_STYLE>
			<< "\n\n";
		} else {
			test_case_results
			<< style<bright<color::GREEN>()>
				<< ident(1) << icon::CHECKMARK << "  " << test_case.description
				<< " (" << test_case.duration << ")"
			<< '\n';
		}
	}

	out_stream << '\n';
	if (some_test_case_failed) {
		out_stream << style<bright<color::RED>()> << icon::CROSS << " ";
	} else {
		out_stream << style<bright<color::GREEN>()> << icon::CHECKMARK << " ";
	}

	out_stream
		<< test_suite.description << ":" << test_case_results.rdbuf()
	<< style<RESET_STYLE>;

	out_stream.flush();
}

void static_terminal::test_case_begun (const test_case& test_case) {
	out_stream
	<< style<font::FAINT>
		<< ident(1) << "discovered test case '" << test_case.description << '\''
	<< style<RESET_STYLE> << endl;
}

void static_terminal::test_case_ended ([[maybe_unused]] const test_case& test_case) {
	/* do nothing */
}

