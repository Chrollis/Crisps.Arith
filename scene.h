#pragma once
#include "graphio.h"
#include "timer.h"

enum SceneType {
	STMainMenu,
	STArithLogin,
	STArithmetic,
	STArithAdmin,
	STGuessNumber,
	STPoints24,
};

class Scene {
protected:
	std::string out;
	std::string feedback;
protected:
	static IMAGE screen;
	static RECT screen_rect;
	static RECT feedback_rect;
	static RECT question_rect;
	static RECT tip_rect;
	static std::vector<int> input_line_position;
	static RECT input_rect;
	static std::string current_id;
public:
	static std::string current_mode;
	static bool current_ended;
public:
	Scene() = default;
	virtual ~Scene() = default;

	virtual void enter() = 0;
	virtual void draw() = 0;
	virtual void timekeep(clock_t delta) = 0;
	virtual void proceed() = 0;
	virtual void input(const ExMessage& msg) = 0;
	virtual void exit() = 0;

	static void enter_loading(const int circle, const std::string& out_str);
	static void greyify_blur_screen(double para = 0.6, int radius = 3, double sigma = 0.7);

protected:
	void normal_draw() const;
	void pop_up_draw() const;
};

