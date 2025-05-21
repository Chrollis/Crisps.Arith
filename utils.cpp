#include "utils.h"

int int_rand(int min, int max) {
	srand(clock() * rand());
	return rand() % (max + 1 - min) + min;
}
std::string get_time_string() {
	std::string str(21, 0);
	time_t now_time;
	tm init = { 0, 0, 0, 1, 0, 1900, 0, 0, -1 };
	tm* time_now = &init;
	time(&now_time);
	localtime_s(time_now, &now_time);
	strftime(&str[0], 21, "%Y/%m/%d, %H:%M:%S", time_now);
	str.pop_back();
	return str;
}
std::wstring string_to_wstring(const std::string& str) {
	if (str.empty()) {
		return L"";
	}
	int size_needed = MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), nullptr, 0);
	if (size_needed == 0) {
		return L"";
	}
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), &wstr[0], size_needed);
	return wstr;
}
bool set_font(HANDLE m_hFont) {
	HRSRC res = FindResourceW(0, MAKEINTRESOURCEW(IDR_FONT1), RT_FONT);
	if (res) {
		HGLOBAL mem = LoadResource(0, res);
		if (!mem) {
			return 0;
		}
		void* data = LockResource(mem);
		DWORD len = SizeofResource(0, res);
		DWORD nFonts = 0;
		m_hFont = AddFontMemResourceEx(data, len, 0, &nFonts);
		if (!m_hFont) {
			return 0;
		}
	}
	return 1;
}

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