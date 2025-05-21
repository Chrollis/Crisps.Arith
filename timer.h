#pragma once
#include "utils.h"

#define WAIT 300

class Timer {
public:
	clock_t timer = 0;
	unsigned counter = 0;
	bool started = 0;
public:
	Timer() = default;
	~Timer() = default;
	clock_t& operator+=(const clock_t delta);
	bool operator()(const bool set);
	bool proceed_loading(std::string& feedback, const unsigned circle = 5, const std::string& feedback_str = "正在处理请求，请稍后。");
};