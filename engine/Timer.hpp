#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

class Timer {
	typedef std::chrono::high_resolution_clock Clock;
	typedef float duration;

	Clock::time_point start;
public:
	Timer() : start(Clock::now()) {}

	void reset() { start = Clock::now(); }

	duration elapsed() const {
		return (Clock::now() - start).count() * 
			((duration)Clock::period::num / Clock::period::den);
	}
};

#endif