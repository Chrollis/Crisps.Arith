#include "guess_number.h"

void GuessNumber::enter() {
	enter_loading(5, "数字炸弹加载中。");
}
void GuessNumber::draw() {
	Graph_IO::set_text_style(L"霞鹜漫黑");
	setbkmode(TRANSPARENT);
	setlinestyle(PS_SOLID, 1);

	if (state < 2) {
		normal_draw();
		if (state == 1) {
			Graph_IO::draw_text(LIMEGREEN, feedback, &feedback_rect,
				DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		}
		else if (mistaken) {
			Graph_IO::draw_text(FIREBRICK3, feedback, &feedback_rect,
				DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		}
	}
	else {
		pop_up_draw();
	}
}
void GuessNumber::proceed(clock_t delta) {
	switch (state) {
	case 0:
		generate_num();
		break;
	case 1:
		check_num(delta);
		break;
	case 2:
		ask_restart();
		break;
	case 3:
		ask_quit();
		break;
	case 4:
		quit(delta);
		break;
	default:
		break;
	}
}
void GuessNumber::input(const ExMessage& msg) {
	if (state == 0) {
		Graph_IO::input_text(msg);
		if (msg.message == WM_KEYDOWN &&
			(msg.vkcode == VK_RETURN ||
				msg.vkcode == VK_SEPARATOR)) {
			state = 1;
			Graph_IO::input.pop_back();
		}
	}
	if (msg.message == WM_KEYDOWN) {
		if (state == 3) {
			if (msg.vkcode == 'Y' ||
				msg.vkcode == VK_RETURN || msg.vkcode == VK_SEPARATOR) {
				state = 4;
			}
			else if (msg.vkcode == 'N' || msg.vkcode == VK_ESCAPE) {
				state = last_state;
				greyified = 0;
			}
		}
		else if (state == 2) {
			if (msg.vkcode == 'Y' ||
				msg.vkcode == VK_RETURN || msg.vkcode == VK_SEPARATOR) {
				state = 0;
				generated = 0;
			}
			else if (msg.vkcode == 'N' || msg.vkcode == VK_ESCAPE) {
				state = 4;
				greyified = 0;
			}
		}
		else if (msg.vkcode == VK_ESCAPE) {
			last_state = state;
			state = 3;
		}
	}
}
void GuessNumber::exit() {
	Graph_IO::input.clear();
}

void GuessNumber::generate_num() {
	if (!generated) {
		do {
			a = int_rand(0, 999);
			b = int_rand(a, 999);
		} while (b - a <= 100);
		c = int_rand(a + 1, b - 1);
		generated = 1;
	}
	out = "请输入一个属于(" + std::to_string(a) + ", " + std::to_string(b) + ")的整数：";
}
void GuessNumber::check_num(clock_t delta) {
	out = "输入结束。";
	if (!Graph_IO::input.empty()) {
		if (!checked) {
			mistaken = 0;
			for (const char c : Graph_IO::input) {
				if (!(c >= '0' && c <= '9')) {
					mistaken = 1;
					break;
				}
			}
			checked = 1;
		}
		if (!mistaken) {
			std::istringstream iss(Graph_IO::input);
			iss >> m;
			if (m > a && m < b) {
				if (proceed_loading(feedback, delta, 5, "判断中。")) {
					if (m != c) {
						if (m < c) {
							a = m;
						}
						else {
							b = m;
						}
						mistaken = 0;
						if (b - a == 2) {
							state = 2;
							succeeded = 1;
							if (!greyified) {
								greyify_blur_screen();
								greyified = 1;
							}
						}
						else {
							mistaken = 0;
							state = 0;
						}
					}
					else {
						mistaken = 0;
						state = 2;
						succeeded = 0;
						if (!greyified) {
							greyify_blur_screen();
							greyified = 1;
						}
					}
					Graph_IO::input.clear();
				}
			}
			else {
				if (proceed_loading(feedback, delta, 5, "判断中。")) {
					mistaken = 1;
					state = 0;
					Graph_IO::input.clear();
					feedback = "输入出界，请重新输入。";
				}
			}
		}
		else {
			if (proceed_loading(feedback, delta, 5)) {
				state = 0;
				checked = 0;
				feedback = "输入有非数字字符。";
				Graph_IO::input.clear();
			}
		}
	}
	else {
		if (proceed_loading(feedback, delta, 5)) {
			mistaken = 1;
			checked = 0;
			state = 0;
			feedback = "输入值不得为空。";
			Graph_IO::input.clear();
		}
	}
}
void GuessNumber::ask_restart() {
	if (succeeded) {
		out = "拆弹成功，是否再来【Y/N】?";
	}
	else {
		out = std::to_string(c) + "号爆炸，是否再来【Y/N】?";
	}
}
void GuessNumber::ask_quit() {
	out = "是否返回主菜单【Y/N】?";
	if (!greyified) {
		greyify_blur_screen();
		greyified = 1;
	}
}
void GuessNumber::quit(clock_t delta) {
	if (proceed_loading(out, delta, 5, "正在返回主菜单。")) {
		current_mode = "menu";
		ended = 1;
	}
}