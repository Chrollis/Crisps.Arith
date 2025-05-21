#include "timer.h"

clock_t& Timer::operator+=(const clock_t delta) {
	if (started) {
		timer += delta;
	}
	return timer;
}
bool Timer::operator()(const bool set) {
	timer = 0;
	counter = 0;
	if (set) {
		started = 1;
	}
	else {
		started = 0;
	}
	return started;
}
bool Timer::proceed_loading(std::string& feedback, const unsigned circle, const std::string& feedback_str) {
	const std::string loading = "/-\\-";
	if (!started) {
		(*this)(1);
	}
	if (timer > WAIT) {
		timer -= WAIT;
		counter += 1;
	}
	if (counter < circle) {
		feedback = feedback_str + loading[counter % 4];
		return 0;
	}
	else {
		return 1;
	}
}