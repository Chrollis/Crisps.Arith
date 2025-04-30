#pragma once
#include "scene.h"

class ArithLogin : public Scene {
public:
	ArithLogin() = default;
	~ArithLogin() = default;

	void enter();
	void draw();
	void proceed(clock_t delta);
	void input(const ExMessage& msg);
	void exit();

private:
	void input_id();
	void check_id(clock_t delta);
	void ask_quit();
	void quit(clock_t delta);
};