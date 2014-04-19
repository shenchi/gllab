#ifndef TIMER_HPP
#define TIMER_HPP

#include <cstdint>

class Timer {
	uint64_t startTime;

	float conversion;
public:
	Timer();

	float now();
};

#endif