#pragma once
#include "scene.h"

class Arithmetic : public Scene {
private:
	int score = 0;
	unsigned quantity = 5;
	struct Bonus {
		unsigned time;
		unsigned range;
	}bonus = { 5000,5 };
	struct Operation {
		bool _switch;
		struct interval {
			int min;
			int max;
		}a, b;
	}operation[4] = {
		{ 1,{0,10},{0,10} },
		{ 1,{0,10},{0,10} },
		{ 0,{0,10},{0,10} },
		{ 0,{0,10},{0,10} }
	};
private:
	COLORREF feedback_color = BLACK;
	int question_count = 0;
	std::string last_question;
	std::string show_score;
	std::string answer;
	int wrong_count = 0;
	clock_t timer = 0;
	bool bonused = 0;
	bool saving = 0;
public:
	Arithmetic() {
		CreateDirectoryW(L".\\data", nullptr);
	}
	~Arithmetic() = default;

	void enter();
	void draw();
	void proceed(clock_t delta);
	void input(const ExMessage& msg);
	void exit();

private:
	void generate_question(clock_t delta);
	void check_answer();
	void settle_score();
	void save_score(clock_t delta);
	void ask_new_turn();
	void ask_quit();
	void quit(clock_t delta);

	bool log(std::string out_str);
	bool read();
	bool save() const;

	static void encrypt(std::string& input, const std::string& key = "password");
	static bool get_password(std::string& password);
	friend class ArithAdmin;
};