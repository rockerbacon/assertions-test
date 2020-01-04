#pragma once

#include "signal_received.h"

namespace test {
	class abort_requested : public signal_received {
		public:
			abort_requested();
	};
}
