#pragma once
#include "scene.h"

class ArithLogin : public Scene {
private:
	int state = InputID;
	int last_state = InputID;
	bool id_check_mistaken = 0;
	bool screenshotted = 0;
	bool id_checked = 0;
	Timer loading;
public:
	ArithLogin() = default;
	~ArithLogin() = default;

	void enter();
	void draw();
	void timekeep(clock_t delta);
	void proceed();
	void input(const ExMessage& msg);
	void exit();
private:
	enum State {
		InputID,
		CheckID,
		AskQuit,
		Quit,
	};
private:
	void input_id();
	void check_id();
	void ask_quit();
	void quit();
};