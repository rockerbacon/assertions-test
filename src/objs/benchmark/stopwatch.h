#pragma once

#include <chrono>
#include <ostream>

namespace benchmark {

	class Stopwatch {
		private:
			std::chrono::high_resolution_clock::time_point lapBegin;
			std::chrono::high_resolution_clock::time_point stopwatchBegin;
		public:
			Stopwatch (void);

			void reset (void);
			std::chrono::high_resolution_clock::duration totalTime (void) const;
			std::chrono::high_resolution_clock::duration lapTime (void) const;
			void newLap (void);
	};


}

std::ostream& operator<< (std::ostream &stream, std::chrono::high_resolution_clock::duration duration);

