#pragma once
#include "scene.h"

class GuessNumber : public Scene {
	int a = 0;
	int b = 0;
	int c = 0;
	int m = 0;
	bool generated = 0;
	bool checked = 0;
public:
	GuessNumber() = default;
	~GuessNumber() = default;

	void enter();
	void draw();
	void proceed(clock_t delta);
	void input(const ExMessage& msg);
	void exit();

	void generate_num();
	void check_num(clock_t delta);
	void ask_restart();
	void ask_quit();
	void quit(clock_t delta);
};