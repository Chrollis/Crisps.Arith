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
	std::string last_question;
	std::string question;
	std::string show_score;
	std::string answer;
	int state = InputAnswer;
	int last_state = InputAnswer;
	int wrong_count = 0;
	int question_count = 0;
	bool answer_check_mistaken = 0;
	bool screenshotted = 0;
	bool answer_checked = 0;
	bool question_generated = 0;
	bool last_wrong = 0;
	bool bonused = 0;
	bool whether_to_save = 0;
	
	Timer thinking;
	Timer loading;
public:
	Arithmetic() {
		CreateDirectoryW(L".\\data", nullptr);
	}
	~Arithmetic() = default;

	void enter();
	void draw();
	void timekeep(clock_t delta);
	void proceed();
	void input(const ExMessage& msg);
	void exit();
private:
	enum State {
		InputAnswer,
		CheckAnswer,
		SettleScore,
		SaveScore,
		AskNewTurn,
		AskQuit,
		Quit
	};
private:
	void generate_question();
	void input_answer();
	void check_answer();
	void settle_score();
	void save_score();
	void ask_new_turn();
	void ask_quit();
	void quit();

	bool log(std::string out_str);
	bool read();
	bool save() const;

	static void encrypt(std::string& input, const std::string& key = "password");
	static bool get_password(std::string& password);
	friend class ArithAdmin;
};