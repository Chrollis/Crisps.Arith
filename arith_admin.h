#pragma once
#include "arithmetic.h"

class ArithAdmin :public Scene {
private:
	std::string variable;
	Arithmetic* current = nullptr;
	bool checked = 0;

public:
	ArithAdmin() = default;
	~ArithAdmin() = default;

	void enter();
	void draw();
	void proceed(clock_t delta);
	void input(const ExMessage& msg);
	void exit();

private:
	void input_password();
	void check_password(clock_t delta);
	void input_id();
	void check_id(clock_t delta);
	void input_variable();
	void check_variable(clock_t delta);
	void input_value();
	void renew_password();
	void check_value(clock_t delta);
	void ask_back();
	void back_user(clock_t delta);
	void back_variable(clock_t delta);
	void ask_quit();
	void quit(clock_t delta);
};