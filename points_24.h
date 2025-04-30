#pragma once
#include "scene.h"

class Points24 :public Scene {
	std::vector<int> numbers;
	std::vector<std::pair<char, double>> nums;
	std::set<std::string> solutions;
	std::string fedback;
	bool solvable = 0;
	bool generated = 0;
	bool checked = 0;
public:
	Points24() = default;
	~Points24() = default;

	void enter();
	void draw();
	void proceed(clock_t delta);
	void input(const ExMessage& msg);
	void exit();

private:
	void generate_num();
	void compute(clock_t delta);
	void ask_regenerate();
	void confirm_solvable(clock_t delta);
	void confirm_restart();
	void ask_quit();
	void quit(clock_t delta);

	bool calculate(double a, double b, char op, double& result);
	void generate_permutations(std::vector<int>& nums, int start, std::vector<std::vector<int>>& result);
	std::vector<std::vector<int>> generate_all_permutations(std::vector<int>& nums);
	bool solve_24(std::vector<int>& nums, std::set<std::string>& solutions);
};