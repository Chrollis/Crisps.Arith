#include "arith_login.h"

void ArithLogin::enter() {
	current_id.clear();
	enter_loading(5, "登陆界面加载中。");
}
void ArithLogin::draw() {
	Graph_IO::set_text_style(L"霞鹜漫黑");
	setbkmode(TRANSPARENT);
	setlinestyle(PS_SOLID, 1);
	if (state == InputID ||
		state == CheckID) {
		normal_draw();
		if (state == CheckID) {
			Graph_IO::draw_text(LIMEGREEN, feedback, &feedback_rect,
				DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		}
		else if (id_check_mistaken) {
			Graph_IO::draw_text(FIREBRICK3, feedback, &feedback_rect,
				DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		}
	}
	else {
		pop_up_draw();
	}
}
void ArithLogin::timekeep(clock_t delta) {
	la.timekeep(delta);
}
void ArithLogin::proceed() {
	switch (state) {
	case InputID:
		input_id();
		break;
	case CheckID:
		check_id();
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
void ArithLogin::input(const ExMessage& msg) {
	if (state == InputID) {
		Graph_IO::input_text(msg);
		if (msg.message == WM_KEYDOWN &&
			(msg.vkcode == VK_RETURN ||
				msg.vkcode == VK_SEPARATOR)) {
			state = CheckID;
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
void ArithLogin::exit() {
	Graph_IO::input.clear();
}

void ArithLogin::input_id() {
	out = "请输入用户ID：";
}
void ArithLogin::check_id() {
	out = "输入结束。";
	if (!id_checked) {
		std::regex rgx("[a-zA-Z0-9_]{1,10}");
		id_check_mistaken = !std::regex_match(Graph_IO::input, rgx);
		id_checked = 1;
	}
	if (id_check_mistaken) {
		if (la.proceed_loading(feedback)) {
			state = InputID;
			la.reset();
			Graph_IO::input.clear();
			id_checked = 0;
			feedback = "ID非法，请重新输入。";
		}
	}
	else {
		if (Graph_IO::input != "admin") {
			if (la.proceed_loading(feedback, 5, "该用户名合法，正在加载。")) {
				current_id = Graph_IO::input;
				current_mode = "arithmetic";
				current_ended = 1;
			}
		}
		else {
			if (la.proceed_loading(feedback, 5, "正在加载管理员系统。")) {
				current_mode = "arith_admin";
				current_ended = 1;
			}
		}
	}
}
void ArithLogin::ask_quit() {
	out = "是否返回主菜单【Y/N】?";
	if (!screenshotted) {
		greyify_blur_screen();
		screenshotted = 1;
	}
}
void ArithLogin::quit() {
	if (la.proceed_loading(out, 5, "正在返回主菜单。")) {
		current_mode = "menu";
		current_ended = 1;
	}
}