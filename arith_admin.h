#pragma once
#include "arithmetic.h"

class ArithAdmin :public Scene {
private:
	std::string variable;
	Arithmetic* current = nullptr;
	int state = InputPassword;
	int last_state = InputPassword;
	bool check_mistaken = 0;
	bool screenshotted = 0;
	bool checked = 0;
	Timer loading;
public:
	ArithAdmin() {
		CreateDirectoryW(L".\\data", nullptr);
	}
	~ArithAdmin() = default;

	void enter();
	void draw();
	void timekeep(clock_t delta);
	void proceed();
	void input(const ExMessage& msg);
	void exit();
private:
	enum State {
		InputPassword,
		CheckPassword,
		InputID,
		CheckID,
		InputVariable,
		CheckVariable,
		InputValue,
		CheckValue,
		AskBack,
		BackUser,
		BackVariable,
		AskQuit,
		Quit
	};
private:
	void input_password();
	void check_password();
	void input_id();
	void check_id();
	void input_variable();
	void check_variable();
	void input_value();
	void renew_password();
	void check_value();
	void ask_back();
	void back_user();
	void back_variable();
	void ask_quit();
	void quit();
};