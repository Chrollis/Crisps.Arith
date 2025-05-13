#include "guess_number.h"

void GuessNumber::enter() {
	enter_loading(5, "数字炸弹加载中。");
}
void GuessNumber::draw() {
	Graph_IO::set_text_style(L"霞鹜漫黑");
	setbkmode(TRANSPARENT);
	setlinestyle(PS_SOLID, 1);

	if (state == InputNum ||
		state == CheckNum) {
		normal_draw();
		if (state == CheckNum) {
			Graph_IO::draw_text(LIMEGREEN, feedback, &feedback_rect,
				DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		}
		else if (num_check_mistaken) {
			Graph_IO::draw_text(FIREBRICK3, feedback, &feedback_rect,
				DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		}
	}
	else {
		pop_up_draw();
	}
}
void GuessNumber::timekeep(clock_t delta) {
	la.timekeep(delta);
}
void GuessNumber::proceed() {
	switch (state) {
	case InputNum:
		input_num();
		break;
	case CheckNum:
		check_num();
		break;
	case AskRestart:
		ask_restart();
		break;
	case AskQuit:
		ask_quit();
		break;
	case Quit:
		quit();
		break;
	default:
		break;
	}
}
void GuessNumber::input(const ExMessage& msg) {
	if (state == InputNum) {
		Graph_IO::input_text(msg);
		if (msg.message == WM_KEYDOWN &&
			(msg.vkcode == VK_RETURN ||
				msg.vkcode == VK_SEPARATOR)) {
			state = CheckNum;
			Graph_IO::input.pop_back();
		}
	}
	if (msg.message == WM_KEYDOWN) {
		if (state == AskQuit) {
			if (msg.vkcode == 'Y' ||
				msg.vkcode == VK_RETURN ||
				msg.vkcode == VK_SEPARATOR) {
				state = Quit;
			}
			else if (msg.vkcode == 'N' ||
				msg.vkcode == VK_ESCAPE) {
				state = last_state;
				screenshotted = 0;
			}
		}
		else if (state == 2) {
			if (msg.vkcode == 'Y' ||
				msg.vkcode == VK_RETURN ||
				msg.vkcode == VK_SEPARATOR) {
				state = InputNum;
				num_generated = 0;
			}
			else if (msg.vkcode == 'N' ||
				msg.vkcode == VK_ESCAPE) {
				state = Quit;
				screenshotted = 0;
			}
		}
		else if (msg.vkcode == VK_ESCAPE) {
			last_state = state;
			state = AskQuit;
		}
	}
}
void GuessNumber::exit() {
	Graph_IO::input.clear();
}

void GuessNumber::generate_num() {
	do {
		a = int_rand(0, 999);
		b = int_rand(a, 999);
	} while (b - a <= 100);
	c = int_rand(a + 1, b - 1);
}
void GuessNumber::input_num() {
	if (!num_generated) {
		generate_num();
		num_generated = 1;
	}
	out = "请输入一个属于(" + std::to_string(a) + ", " + std::to_string(b) + ")的整数：";
}
void GuessNumber::check_num() {
	out = "输入结束。";
	if (!num_checked) {
		std::regex rgx("[0-9]{1,10}");
		num_check_mistaken = !std::regex_match(Graph_IO::input, rgx);
		num_checked = 1;
	}
	if (!num_check_mistaken) {
		std::istringstream iss(Graph_IO::input);
		iss >> m;
		if (m > a && m < b) {
			if (la.proceed_loading(feedback, 5, "判断中。")) {
				if (m != c) {
					if (m < c) {
						a = m;
					}
					else {
						b = m;
					}
					if (b - a == 2) {
						state = AskRestart;
						succeeded = 1;
						if (!screenshotted) {
							greyify_blur_screen();
							screenshotted = 1;
						}
					}
					else {
						state = InputNum;
						num_checked = 0;
					}
				}
				else {
					state = AskRestart;
					succeeded = 0;
					if (!screenshotted) {
						greyify_blur_screen();
						screenshotted = 1;
					}
				}
				la.reset();
				Graph_IO::input.clear();
			}
		}
		else {
			if (la.proceed_loading(feedback, 5, "判断中。")) {
				num_checked = 0;
				num_check_mistaken = 1;
				state = InputNum;
				la.reset();
				Graph_IO::input.clear();
				feedback = "输入出界，请重新输入。";
			}
		}
	}
	else {
		if (la.proceed_loading(feedback)) {
			state = InputNum;
			la.reset();
			num_checked = 0;
			feedback = "非法输入，请重新输入。";
			Graph_IO::input.clear();
		}
	}
}
void GuessNumber::ask_restart() {
	if (succeeded) {
		out = std::to_string(c) + "号已拆除，是否再来【Y/N】?";
	}
	else {
		out = std::to_string(c) + "号已爆炸，是否再来【Y/N】?";
	}
}
void GuessNumber::ask_quit() {
	out = "是否返回主菜单【Y/N】?";
	if (!screenshotted) {
		greyify_blur_screen();
		screenshotted = 1;
	}
}
void GuessNumber::quit() {
	if (la.proceed_loading(out, 5, "正在返回主菜单。")) {
		current_mode = "menu";
		current_ended = 1;
	}
}