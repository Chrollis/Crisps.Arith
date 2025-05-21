#pragma once
#include "scene.h"

class MainMenu :public Scene {
private:
	int state = InputMode;
	int last_state = InputMode;
	bool mode_check_mistaken = 0;
	bool screenshotted = 0;
	bool mode_checked = 0;
	Timer loading;
public:
	MainMenu() = default;
	~MainMenu() = default;

	void enter();
	void draw();
	void timekeep(clock_t delta);
	void proceed();
	void input(const ExMessage& msg);
	void exit();
private:
	enum State {
		InputMode,
		CheckMode,
		AskQuit,
		Quit,
	};
private:
	void input_mode();
	void check_mode();
	void ask_quit();
	void quit();
};