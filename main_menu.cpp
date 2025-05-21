#include "main_menu.h"

void MainMenu::enter() {
	current_mode.clear();
	enter_loading(5, "�˵���������С�");
}
void MainMenu::draw() {
	Graph_IO::set_text_style(L"ϼ������");
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
	out = "������ģʽ����";
}
void MainMenu::check_mode() {
	out = "���������";
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
		if (loading.proceed_loading(feedback, 5, "��ģʽ���ڣ�������ת��")) {
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
			feedback = "��ģʽ�����ڣ����������롣";
		}
	}
}
void MainMenu::ask_quit() {
	out = "�Ƿ��˳�����Y/N��?";
	if (!screenshotted) {
		greyify_blur_screen();
		screenshotted = 1;
	}
}
void MainMenu::quit() {
	if (loading.proceed_loading(out, 5, "�����˳���")) {
		current_mode = "exit";
		current_ended = 1;
	}
}