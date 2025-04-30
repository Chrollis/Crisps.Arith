#include "main_menu.h"

void MainMenu::enter() {
	current_mode.clear();
	enter_loading(5, "菜单界面加载中。");
}
void MainMenu::draw() {
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
void MainMenu::proceed(clock_t delta) {
	switch (state) {
	case 0:
		input_mode();
		break;
	case 1:
		check_mode(delta);
		break;
	case 2:
		ask_quit();
		break;
	case 3:
		quit(delta);
		break;
	default:
		break;
	}
}
void MainMenu::input(const ExMessage& msg) {
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
		if (state == 2) {
			if (msg.vkcode == 'Y' ||
				msg.vkcode == VK_RETURN || msg.vkcode == VK_SEPARATOR) {
				state = 3;
			}
			else if (msg.vkcode == 'N' || msg.vkcode == VK_ESCAPE) {
				state = last_state;
				greyified = 0;
			}
		}
		else if (msg.vkcode == VK_ESCAPE) {
			last_state = state;
			state = 2;
		}
	}
}
void MainMenu::exit() {
	Graph_IO::input.clear();
}

void MainMenu::input_mode() {
	out = "请输入模式名：";
}
void MainMenu::check_mode(clock_t delta) {
	out = "输入结束。";
	std::vector<std::string>legal_mode = {
	"kousuan",
	"caishuzi",
	"24dian"
	};
	if (!checked) {
		mistaken = 1;
		for (const std::string& mode : legal_mode) {
			if (Graph_IO::input == mode) {
				mistaken = 0;
				break;
			}
		}
		checked = 1;
	}
	if (!mistaken) {
		if (proceed_loading(feedback, delta, 5, "该模式存在，正在跳转。")) {
			current_mode = Graph_IO::input;
			ended = 1;
		}
	}
	else {
		if (proceed_loading(feedback, delta)) {
			state = 0;
			checked = 0;
			Graph_IO::input.clear();
			feedback = "该模式不存在，请重新输入。";
		}
	}
}
void MainMenu::ask_quit() {
	out = "是否退出程序【Y/N】?";
	if (!greyified) {
		greyify_blur_screen();
		greyified = 1;
	}
}
void MainMenu::quit(clock_t delta) {
	if (proceed_loading(out, delta, 5, "正在退出。")) {
		current_mode = "exit";
		ended = 1;
	}
}