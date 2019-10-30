#pragma once

#if __cplusplus >= 201703L
	#define NO_UNUSED_WARNING [[maybe_unused]]
#else
	#ifdef __GNUC__
		#define NO_UNUSED_WARNING __attribute__((unused))
	#else
		#define NO_UNUSED_WARNING
	#endif
#endif

