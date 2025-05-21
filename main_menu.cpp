#include "main_menu.h"

void MainMenu::enter() {
	current_mode.clear();
	enter_loading(5, "菜单界面加载中。");
}
void MainMenu::draw() {
	Graph_IO::set_text_style(L"霞鹜漫黑");
	setbkmode(TRANSPARENT);
	setlinestyle(PS_SOLID, 1);

	if (state == InputMode ||
		state == CheckMode) {
		normal_draw();
		if (state == CheckMode) {
			Graph_IO::draw_text(LIMEGREEN, feedback, &feedback_rect,
				DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		}
		else if (mode_check_mistaken) {
			Graph_IO::draw_text(FIREBRICK3, feedback, &feedback_rect,
				DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		}
	}
	else {
		pop_up_draw();
	}
}
void MainMenu::timekeep(clock_t delta) {
	loading += delta;
}
void MainMenu::proceed() {
	switch (state) {
	case InputMode:
		input_mode();
		break;
	case CheckMode:
		check_mode();
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
void MainMenu::input(const ExMessage& msg) {
	if (state == InputMode) {
		Graph_IO::input_text(msg);
		if (msg.message == WM_KEYDOWN &&
			(msg.vkcode == VK_RETURN ||
				msg.vkcode == VK_SEPARATOR)) {
			state = CheckMode;
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
		else if (msg.vkcode == VK_ESCAPE) {
			last_state = state;
			state = AskQuit;
		}
	}
}
void MainMenu::exit() {
	Graph_IO::input.clear();
}

void MainMenu::input_mode() {
	out = "请输入模式名：";
}
void MainMenu::check_mode() {
	out = "输入结束。";
	std::vector<std::string>legal_mode = {
	"kousuan",
	"caishuzi",
	"24dian"
	};
	if (!mode_checked) {
		mode_check_mistaken = 1;
		for (const std::string& mode : legal_mode) {
			if (Graph_IO::input == mode) {
				mode_check_mistaken = 0;
				break;
			}
		}
		mode_checked = 1;
	}
	if (!mode_check_mistaken) {
		if (loading.proceed_loading(feedback, 5, "该模式存在，正在跳转。")) {
			current_mode = Graph_IO::input;
			current_ended = 1;
		}
	}
	else {
		if (loading.proceed_loading(feedback)) {
			state = InputMode;
			loading(0);
			Graph_IO::input.clear();
			mode_checked = 0;
			feedback = "该模式不存在，请重新输入。";
		}
	}
}
void MainMenu::ask_quit() {
	out = "是否退出程序【Y/N】?";
	if (!screenshotted) {
		greyify_blur_screen();
		screenshotted = 1;
	}
}
void MainMenu::quit() {
	if (loading.proceed_loading(out, 5, "正在退出。")) {
		current_mode = "exit";
		current_ended = 1;
	}
}