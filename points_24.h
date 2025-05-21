#pragma once
#include "scene.h"

class Points24 :public Scene {
	std::vector<int> numbers;
	std::vector<std::pair<char, double>> nums;
	std::set<std::string> solutions;
	std::string feedbackA;
	int state = InputNum;
	int last_state = InputNum;
	bool input_check_mistaken = 0;
	bool screenshotted = 0;
	bool solvable = 0;
	bool num_generated = 0;
	bool input_checked = 0;
	bool solvable_checked = 0;
	bool letter_or_digit = Letter;
	Timer loading;
public:
	Points24() = default;
	~Points24() = default;

	void enter();
	void draw();
	void timekeep(clock_t delta);
	void proceed();
	void input(const ExMessage& msg);
	void exit();
private:
	enum State {
		InputNum,
		Compute,
		AskRegenerate,
		ConfirmSolvable,
		ConfirmRestart,
		AskQuit,
		Quit,
	};
	enum LetterOrDigit {
		Letter,
		Digit
	};
private:
	void generate_num();
	void input_num();
	void compute();
	void ask_regenerate();
	void confirm_solvable();
	void confirm_restart();
	void ask_quit();
	void quit();

	bool calculate(double a, double b, char op, double& result);
	void generate_permutations(std::vector<int>& nums, int start, std::vector<std::vector<int>>& result);
	std::vector<std::vector<int>> generate_all_permutations(std::vector<int>& nums);
	bool solve_24(std::vector<int>& nums, std::set<std::string>& solutions);
};