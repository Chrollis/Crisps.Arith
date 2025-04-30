#include "arith_login.h"

void ArithLogin::enter() {
	current_id.clear();
	enter_loading(5, "��½��������С�");
}
void ArithLogin::draw() {
	Graph_IO::set_text_style(L"ϼ������");
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
void ArithLogin::proceed(clock_t delta) {
	switch (state) {
	case 0:
		input_id();
		break;
	case 1:
		check_id(delta);
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
void ArithLogin::input(const ExMessage& msg) {
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
void ArithLogin::exit() {
	Graph_IO::input.clear();
}

void ArithLogin::input_id() {
	out = "�������û�ID��";
}
void ArithLogin::check_id(clock_t delta) {
	out = "���������";
	if (Graph_IO::input.length() > 10) {
		if (proceed_loading(feedback, delta)) {
			mistaken = 1;
			state = 0;
			Graph_IO::input.clear();
			feedback = "ID���������������롣";
		}
	}
	else if (Graph_IO::input.empty()) {
		if (proceed_loading(feedback, delta)) {
			mistaken = 1;
			state = 0;
			Graph_IO::input.clear();
			feedback = "ID����Ϊ�գ����������롣";
		}
	}
	else {
		if (Graph_IO::input != "admin") {
			if (proceed_loading(feedback, delta, 5, "���û����Ϸ������ڼ��ء�")) {
				current_id = Graph_IO::input;
				current_mode = "arithmetic";
				ended = 1;
			}
		}
		else {
			if (proceed_loading(feedback, delta, 5, "���ڼ��ع���Աϵͳ��")) {
				current_mode = "arith_admin";
				ended = 1;
			}
		}
	}
}
void ArithLogin::ask_quit() {
	out = "�Ƿ񷵻����˵���Y/N��?";
	if (!greyified) {
		greyify_blur_screen();
		greyified = 1;
	}
}
void ArithLogin::quit(clock_t delta) {
	if (proceed_loading(out, delta, 5, "���ڷ������˵���")) {
		current_mode = "menu";
		ended = 1;
	}
}