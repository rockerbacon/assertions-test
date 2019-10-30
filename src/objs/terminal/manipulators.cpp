#include <terminal/manipulators.h>

using namespace std;
using namespace terminal;

manipulator terminal::cursor_up (unsigned offset) {
	return [offset](auto& stream) {
		stream << "\E[" << offset << 'A';
	};
}

manipulator terminal::cursor_down (unsigned offset) {
	return [offset](auto& stream) {
		stream << "\E[" << offset << 'B';
	};
}

manipulator terminal::cursor_left(unsigned offset) {
	return [offset](auto& stream) {
		stream << "\E[" << offset << 'D';
	};
}

manipulator terminal::cursor_right(unsigned offset) {
	return [offset](auto& stream) {
		stream << "\E[" << offset << 'C';
	};
}

ostream& terminal::save_cursor_position(ostream &stream) {
	stream << "\E[s";
	return stream;
}

ostream& terminal::restore_cursor_position(ostream &stream) {
	stream << "\E[u";
	return stream;
}

ostream& terminal::show_cursor(ostream &stream) {
	stream << "\E[?25h";
	return stream;
}

ostream& terminal::hide_cursor(ostream &stream) {
	stream << "\E[?25l";
	return stream;
}

ostream& terminal::clear_line(ostream& stream) {
	stream << "\E[2K";
	return stream;
}

manipulator terminal::ident (unsigned size) {
	return [size](auto& stream) {
		for (decltype(size) i = 0; i < size; i++)
		   stream << '\t';
	};
}

ostream& operator<< (ostream& stream, const manipulator& manipulator) {
	manipulator(stream);
	return stream;
}

