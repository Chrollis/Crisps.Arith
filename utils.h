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

int int_rand(int min, int max);
std::string get_time_string();
std::wstring string_to_wstring(const std::string& str);
#define string_to_lpcwstr(str) string_to_wstring(str).c_str()
bool set_font(HANDLE m_hFont);