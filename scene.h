#pragma once
#include "graph_io.h"

#define WAIT 300
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
	int state = 0;
	int last_state = 0;
	bool mistaken = 0;
	bool greyified = 0;
	std::string out;
	std::string feedback;

	static IMAGE screen;
	static RECT screen_rect;
	static RECT feedback_rect;
	static RECT question_rect;
	static std::vector<int> input_line_position;
	static RECT input_rect;
	static std::string current_id;
public:
	static std::string current_mode;
	static bool ended;
public:
	Scene() = default;
	virtual ~Scene() = default;

	virtual void enter() = 0;
	virtual void draw() = 0;
	virtual void proceed(clock_t delta) = 0;
	virtual void input(const ExMessage& msg) = 0;
	virtual void exit() = 0;

	static void enter_loading(const int circle, const std::string& out_str);
	static bool proceed_loading(std::string& feedback, clock_t delta, const int circle = 5, const std::string& feedback_str = "正在处理请求，请稍后。");
	static void greyify_blur_screen(double para = 0.6, int radius = 3, double sigma = 0.7);

protected:
	void normal_draw();
	void pop_up_draw();
};