#include "json_logger.h"

#include <ostream>
#include <stopwatch/stopwatch.h>

using namespace test;
using namespace std;

json_logger::json_logger(ostream& out_stream) :
	out_stream(out_stream)
{}

void json_logger::tests_begun () {
	/* do nothing */
}

void json_logger::tests_ended (unsigned failed_tests, unsigned successful_tests) {
	out_stream <<
	"{"
		R"("event":"tests_ended",)"
		R"("successful_tests":)" << successful_tests << ","
		R"("failed_tests":)" << failed_tests <<
	"}" << endl;
}

void json_logger::test_suite_begun (const test_suite& test_suite) {
	this->out_stream <<
	"{"
		R"("event":"test_suite_begun",)"
		R"("test_suite":)"
		"{"
			R"("id":)"				<< test_suite.id << ","
			R"("description":")"	<< test_suite.description << "\""
		"}"
	"}" << endl;
}

void json_logger::test_suite_ended (const test_suite& test_suite) {
	out_stream <<
	"{"
		R"("status":"test_suite_ended",)"
		R"("test_suite":)"
		"{"
			R"("id":)"				<< test_suite.id << ","
			R"("description:")"		<< test_suite.description << "\","
			R"("test_cases:")"
			"[";
				auto final_test_case = test_suite.test_cases.end();
				final_test_case--;
				for (
					auto it = test_suite.test_cases.begin();
				   	it != test_suite.test_cases.end();
					it++
				) {
					out_stream <<
					"{"
						R"("description":")"	<< it->description << "\","
						R"("duration":)"		<< it->duration.count() << ","
						R"("status":)"			<< it->status <<
					"}";
					if (it != final_test_case)
						out_stream << ",";
				}
			out_stream <<
			"]"
		"}"
	"}" << endl;
}


void json_logger::test_case_begun (const test_case& test_case) {
	out_stream <<
	"{"
		R"("event":"test_case_begun",)"
		R"("test_case":")"
		"{"
			R"("description":")"	<< test_case.description << "\","
			R"("test_suite_id":)"	<< test_case.test_suite.id <<
		"}"
	"}" << endl;
}

void json_logger::test_case_ended (const test_case& test_case) {
	out_stream <<
	"{"
		R"("status":"test_case_ended",)"
		R"("test_case":)"
		"{"
			R"("description":")"	<< test_case.description << "\","
			R"("duration":)"		<< test_case.duration.count() << ","
			R"("status":)" 		<< test_case.status << ","
			R"("failure_reason":")"	<< test_case.failure_reason << "\","
			R"("test_suite_id":)"	<< test_case.test_suite.id <<
		"}"
	"}" << endl;
}

