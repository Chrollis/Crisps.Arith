#pragma once
#include "main_menu.h"
#include "arith_login.h"
#include "arith_admin.h"
#include "guess_number.h"
#include "points_24.h"

class FpsController {
private:
	int interval = 83;
	int fps_target = 12;
	clock_t begin = 0;
	clock_t end = 0;
	int actual_fps = 0;
public:
	FpsController() = default;
	~FpsController() = default;

	void start();
	void delay()const;
	void set(const int fps);
	void draw();
};

class SceneController {
private:
	Scene* current = nullptr;
public:
	SceneController() = default;
	~SceneController() = default;

	void switch_list(SceneType type);
	void set(SceneType type);
	void switch_to(SceneType type);

	void draw();
	void proceed(clock_t delta);
	void input(const ExMessage& msg);
};