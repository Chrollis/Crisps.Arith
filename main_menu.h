#pragma once
#include "scene.h"

class MainMenu :public Scene {
private:
	bool checked = 0;
public:
	MainMenu() = default;
	~MainMenu() = default;

	void enter();
	void draw();
	void proceed(clock_t delta);
	void input(const ExMessage& msg);
	void exit();

private:
	void input_mode();
	void check_mode(clock_t delta);
	void ask_quit();
	void quit(clock_t delta);
};