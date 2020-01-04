#pragma once

#include "signal_received.h"

namespace test {
	class segmentation_fault : public signal_received {
		public:
			segmentation_fault();
	};
}
