#include "live_terminal.h"
#include "../terminal/manipulators.h"
#include <iostream>
#include <stopwatch/stopwatch.h>

using namespace std;
using namespace test;
using namespace terminal;

live_terminal::live_terminal (void)
	:	out_stream(cout),
		current_depth(0),
		depth_map()
{}

void live_terminal::tests_begun (void) {
	this->out_stream << save_cursor_position << hide_cursor;
}

void live_terminal::test_suite_block_begun (const string& test_suite_description) {
	this->out_stream
		<< restore_cursor_position << cursor_down(this->depth_map.size())
			<< ident(this->current_depth)
			<< test_suite_description << ':'
		<< '\n';

	this->depth_map.push_back(this->current_depth);

	this->out_stream
		<< cursor_up(this->depth_map.size()) << save_cursor_position
		<< cursor_down(this->depth_map.size());

	this->current_depth++;
}

void live_terminal::test_case_discovered (const string& test_case_description) {
	this->out_stream
		<< restore_cursor_position << cursor_down(this->depth_map.size())
			<< ident(this->current_depth)

			<< style<font::FAINT>
				<< icon::CIRCLE << "  "

				<< test_case_description
			<< style<RESET_STYLE>
		<< '\n';

	this->depth_map.push_back(this->current_depth);

	this->out_stream
		<< cursor_up(this->depth_map.size()) << save_cursor_position
		<< cursor_down(this->depth_map.size());
}

void live_terminal::test_suite_block_ended (void) {
	this->current_depth--;
}

void live_terminal::test_case_execution_begun (const std::string& test_case_description, unsigned row) {
	this->out_stream
		<< restore_cursor_position << cursor_down(row) << clear_line
			<< ident(this->depth_map[row])

			<< style<font::FAINT>
				<< style<font::BLINK>
					<< icon::CIRCLE << "  "
				<< style<RESET_STYLE> << style<font::FAINT>

				<< test_case_description
			<< style<RESET_STYLE>
		<< '\n';
}

void live_terminal::test_case_failed (const std::string& test_case_description, unsigned row, chrono::high_resolution_clock::duration test_duration, const std::string& reason) {
	this->out_stream
		<< restore_cursor_position << cursor_down(row) << clear_line
			<< ident(this->depth_map[row])

			<< style<bright<color::RED>()>
				<< icon::CIRCLE << "  " << test_case_description
				<< ": " << reason
				<< " (" << test_duration << ")"
		<< '\n';
}

void live_terminal::test_case_succeeded (const string& test_case_description, unsigned row, chrono::high_resolution_clock::duration test_duration) {
	this->out_stream
		<< restore_cursor_position	<< cursor_down(row) << clear_line
			<< ident(this->depth_map[row])

			<< style< bright<color::GREEN>() >
				<< icon::CIRCLE << "  " << test_case_description
				<< " (" << test_duration << ")"
		<< '\n';
}

void live_terminal::tests_ended ([[maybe_unused]] unsigned successful_tests, [[maybe_unused]] unsigned failed_tests) {
	this->out_stream
		<< restore_cursor_position << cursor_down(this->depth_map.size())
		<< style<RESET_STYLE>
		<< show_cursor
		<< endl;
}

