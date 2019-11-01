#include "json_logger.h"

#include <ostream>
#include <stopwatch/stopwatch.h>

using namespace test;
using namespace std;

json_logger::json_logger(ostream& out_stream)
	:	out_stream(out_stream)
{}

void json_logger::tests_begun (void) {
	this->out_stream
		<< '{'
			<< "\"status\":\"begun\""
		<< '}' << endl;
}

void json_logger::test_suite_block_begun (const std::string& test_suite_description) {
	this->out_stream
		<< '{'
			<< "\"status\":\"test_suite_block_begun\","
			<< "\"test_suite\":"
			<< '{'
				<< "\"description\":\"" << test_suite_description << '\"'
			<< '}'
		<< '}' << endl;
}

void json_logger::test_case_discovered (const std::string& test_case_description) {
	this->out_stream
		<< '{'
			<< "\"status\":\"test_case_discovered\","
			<< "\"test_case\":"
			<< '{'
				<< "\"description\":\"" << test_case_description << '\"'
			<< '}'
		<< '}' << endl;
}

void json_logger::test_suite_block_ended (void) {
	this->out_stream
		<< '{'
			<< "\"status\":\"test_suite_block_ended\","
		<< '}' << endl;
}

void json_logger::test_case_execution_begun (const string& test_case_description, unsigned row) {
	this->out_stream
		<< '{'
			<< "\"status\":\"test_case_execution_begun\","
			<< "\"test_case\":"
			<< '{'
				<< "\"description\":\"" << test_case_description << "\","
				<< "\"row\":" << row
			<< '}'
		<< '}' << endl;
}

void json_logger::test_case_failed (const string &test_case_description, unsigned row, chrono::high_resolution_clock::duration test_duration, const string& reason) {
	this->out_stream
		<< '{'
			<< "\"status\":\"test_case_failed\","
			<< "\"test_case\":"
			<< '{'
				<< "\"description\":\"" << test_case_description << "\","
				<< "\"row\":" << row << ','
				<< "\"duration\":\"" << test_duration << "\","
				<< "\"failure_reason\":\"" << reason << '\"'
			<< '}'
		<< '}' << endl;
}

void json_logger::test_case_succeeded (const string& test_case_description, unsigned row, chrono::high_resolution_clock::duration test_duration) {
	this->out_stream
		<< '{'
			<< "\"status\":\"test_case_succeeded\","
			<< "\"test_case\":"
			<< '{'
				<< "\"description\":\"" << test_case_description << "\","
				<< "\"row\":" << row << ','
				<< "\"duration\":\"" << test_duration << '\"'
			<< '}'
		<< '}' << endl;
}

void json_logger::tests_ended (unsigned successful_tests, unsigned failed_tests) {
	this->out_stream
		<< '{'
			<< "\"status\":\"ended\","
			<< "\"successful_tests\":" << successful_tests << ","
			<< "\"failed_tests\":" << failed_tests
		<< '}' << endl;
}
