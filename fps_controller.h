#pragma once
#include "timer.h"
#include "graphio.h"

#define FPS 24

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