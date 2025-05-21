#pragma once
#include <graphics.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <cmath>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <regex>
#include "resource.h"

#define M_PI 3.1415926
#define EPSILON 1e-6

#define FPS 24
#define WAIT 300

int int_rand(int min, int max);
std::string get_time_string();
std::wstring string_to_wstring(const std::string& str);
#define string_to_lpcwstr(str) string_to_wstring(str).c_str()
bool set_font(HANDLE m_hFont);

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