#pragma once
#include "scene.h"

class GuessNumber : public Scene {
private:
	int a = 0;
	int b = 0;
	int c = 0;
	int m = 0;
	int state = InputNum;
	int last_state = InputNum;
	bool num_check_mistaken = 0;
	bool screenshotted = 0;
	bool num_checked = 0;
	bool num_generated = 0;
	bool succeeded = 0;
	Loading la;
public:
	GuessNumber() = default;
	~GuessNumber() = default;

	void enter();
	void draw();
	void timekeep(clock_t delta);
	void proceed();
	void input(const ExMessage& msg);
	void exit();
private:
	enum State {
		InputNum,
		CheckNum,
		AskRestart,
		AskQuit,
		Quit,
	};
private:
	void generate_num();
	void input_num();
	void check_num();
	void ask_restart();
	void ask_quit();
	void quit();
};