#include "Timer.hpp"
#include <mach/mach_time.h>

Timer::Timer() {
	startTime = mach_absolute_time();

	mach_timebase_info_data_t info;
	mach_timebase_info(&info);
	conversion = 1e-9 * (float)info.numer / (float)info.denom;
}

float Timer::now() {
	uint64_t nowTime = mach_absolute_time();
	return conversion * (float)(nowTime - startTime);
}