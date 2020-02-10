#pragma once

#include <ostream>
#include <functional>

namespace terminal {

	typedef unsigned short style_code;
	typedef const char* unicode_char;
	typedef std::function<void(std::ostream&)> manipulator;


	namespace color {
		constexpr style_code BLACK 		= 30;
		constexpr style_code RED		= 31;
		constexpr style_code GREEN 		= 32;
		constexpr style_code BROWN 		= 33;
		constexpr style_code BLUE 		= 34;
		constexpr style_code MAGENTA 	= 35;
		constexpr style_code CYAN		= 36;
		constexpr style_code WHITE		= 37;
		constexpr style_code DEFAULT 	= 39;
	};

	namespace font {
		constexpr style_code BOLD		= 1;
		constexpr style_code FAINT		= 2;
		constexpr style_code ITALIC		= 3;
		constexpr style_code UNDERLINE	= 4;
		constexpr style_code BLINK		= 5;
	};

	namespace icon {
		constexpr unicode_char CIRCLE		= "\u25CF";
		constexpr unicode_char CHECKMARK	= "\u2713";
		constexpr unicode_char CROSS		= "\u2613";
	};

	constexpr style_code RESET_STYLE = 0;

	template<style_code code>
	constexpr style_code bright(void) {
		static_assert(code >= 30 && code <= 37, "either not a color or already bright");
		return code+60;
	}

	template<style_code code>
	std::ostream& style (std::ostream& stream) {
		stream << "\E[" << code << 'm';
		return stream;
	}

	manipulator cursor_up (unsigned offset);
	manipulator cursor_down (unsigned offset);
	manipulator cursor_left (unsigned offset);
	manipulator cursor_right (unsigned offset);

	std::ostream& save_cursor_position (std::ostream& stream);
	std::ostream& restore_cursor_position (std::ostream& stream);

	std::ostream& show_cursor (std::ostream& stream);
	std::ostream& hide_cursor (std::ostream& stream);

	std::ostream& clear_line (std::ostream& stream);

	manipulator ident (unsigned size);
}

std::ostream& operator<< (std::ostream& stream, const terminal::manipulator& manipulator);

