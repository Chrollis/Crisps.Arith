#include "arith_admin.h"

void ArithAdmin::enter() {
	enter_loading(5, "����Ա��������С�");
}
void ArithAdmin::draw() {
	Graph_IO::set_text_style(L"ϼ������");
	setbkmode(TRANSPARENT);
	setlinestyle(PS_SOLID, 1);
	if (state < 8) {
		normal_draw();
		if (state % 2 == 1) {
			Graph_IO::draw_text(LIMEGREEN, feedback, &feedback_rect,
				DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		}
		else if (state == 6) {
			Graph_IO::draw_text(LIMEGREEN, feedback, &feedback_rect,
				DT_LEFT | DT_VCENTER | DT_SINGLELINE);
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
void ArithAdmin::proceed(clock_t delta) {
	switch (state)
	{
	case 0:
		input_password();
		break;
	case 1:
		check_password(delta);
		break;
	case 2:
		input_id();
		break;
	case 3:
		check_id(delta);
		break;
	case 4:
		input_variable();
		break;
	case 5:
		check_variable(delta);
		break;
	case 6:
		input_value();
		break;
	case 7:
		check_value(delta);
		break;
	case 8:
		ask_back();
		break;
	case 9:
		back_user(delta);
		break;
	case 10:
		back_variable(delta);
		break;
	case 11:
		ask_quit();
		break;
	case 12:
		quit(delta);
		break;
	default:
		break;
	}
}
void ArithAdmin::input(const ExMessage& msg) {
	if (state < 8 && state % 2 == 0) {
		Graph_IO::input_text(msg);
		if (msg.message == WM_KEYDOWN &&
			(msg.vkcode == VK_RETURN ||
				msg.vkcode == VK_SEPARATOR)) {
			state += 1;
			Graph_IO::input.pop_back();
		}
	}
	if (msg.message == WM_KEYDOWN) {
		if (state == 11) {
			if (msg.vkcode == 'Y' ||
				msg.vkcode == VK_RETURN || msg.vkcode == VK_SEPARATOR) {
				state = 12;
			}
			else if (msg.vkcode == 'N' || msg.vkcode == VK_ESCAPE) {
				state = last_state;
				if (state != 8) {
					greyified = 0;
				}
			}
		}
		else if (state == 8) {
			if (msg.vkcode == 'U') {
				state = 9;
				greyified = 0;
			}
			else if (msg.vkcode == 'V') {
				state = 10;
				greyified = 0;
			}
			else if(msg.vkcode == VK_ESCAPE) {
				last_state = state;
				state = 11;
			}
		}
		else if (msg.vkcode == VK_ESCAPE) {
			last_state = state;
			state = 11;
		}
	}
}
void ArithAdmin::exit() {
	delete current;
	Graph_IO::input.clear();
}

void ArithAdmin::input_password() {
	out = "���������룺";
}
void ArithAdmin::check_password(clock_t delta) {
	out = "���������";
	std::string password;
	Arithmetic::get_password(password);
	if (Graph_IO::input == password) {
		if (proceed_loading(feedback, delta, 5, "������ȷ�������ض���")) {
			state = 2;
			Graph_IO::input.clear();
		}
	}
	else {
		if (proceed_loading(feedback, delta, 5, "������󣬳�����ֹ��")) {
			current_mode = "exit";
			ended = 1;
		}
	}
}
void ArithAdmin::input_id() {
	out = "������Ŀ��ID��";
}
void ArithAdmin::check_id(clock_t delta) {
	out = "���������";
	if (Graph_IO::input.length() > 10) {
		if (proceed_loading(feedback, delta)) {
			mistaken = 1;
			state = 2;
			Graph_IO::input.clear();
			feedback = "ID���������������롣";
		}
	}
	else if (Graph_IO::input.empty()) {
		if (proceed_loading(feedback, delta)) {
			mistaken = 1;
			state = 2;
			Graph_IO::input.clear();
			feedback = "ID����Ϊ�գ����������롣";
		}
	}
	else {
		if (Graph_IO::input != "admin") {
			if (proceed_loading(feedback, delta, 5, "���û����Ϸ������ڼ��ء�")) {
				current = new Arithmetic();
				current_id = Graph_IO::input;
				if (current->read()) {
					current->log("started");
				}
				else {
					current->save();
					current->log("created");
				}
				Graph_IO::input.clear();
				mistaken = 0;
				state = 4;
			}
		}
		else {
			if (proceed_loading(feedback, delta)) {
				mistaken = 1;
				state = 2;
				Graph_IO::input.clear();
				feedback = "�����ظ��������ԱID��";
			}
		}
	}
}
void ArithAdmin::input_variable() {
	out = "������������ƣ�";
}
void ArithAdmin::check_variable(clock_t delta) {
	out = "���������";
	std::vector<std::string>legal_varible = {
	"score",
	"quantity",
	"bonus",
	"bonus.time",
	"bonus.range",
	"add",
	"add._switch",
	"add.a",
	"add.a.min",
	"add.a.max",
	"add.b",
	"add.b.min",
	"add.b.max",
	"sub",
	"sub._switch",
	"sub.a",
	"sub.a.min",
	"sub.a.max",
	"sub.b",
	"sub.b.min",
	"sub.b.max",
	"mul",
	"mul._switch",
	"mul.a",
	"mul.a.min",
	"mul.a.max",
	"mul.b",
	"mul.b.min",
	"mul.b.max",
	"div",
	"div._switch",
	"div.a",
	"div.a.min",
	"div.a.max",
	"div.b",
	"div.b.min",
	"div.b.max",
	"password"
	};
	if (!checked) {
		mistaken = 1;
		for (const std::string& var : legal_varible) {
			if (Graph_IO::input == var) {
				mistaken = 0;
				break;
			}
		}
		checked = 1;
	}
	if (!mistaken) {
		if (proceed_loading(feedback, delta, 5, "�ñ������ڣ�������ת��")) {
			variable = Graph_IO::input;
			Graph_IO::input.clear();
			checked = 0;
			state = 6;
		}
	}
	else {
		if (proceed_loading(feedback, delta)) {
			state = 4;
			checked = 0;
			Graph_IO::input.clear();
			feedback = "�ñ��������ڣ����������롣";
		}
	}
}
void ArithAdmin::input_value() {
	out = "������Ŀ��ֵ��";
	std::ostringstream oss;
	size_t length = variable.length();
	std::string password;
	Arithmetic::get_password(password);
	auto switch_operation = [&](const size_t i) {
		if (length == 3) {
			oss << current->operation[i]._switch << ", "
				<< current->operation[i].a.min << ", "
				<< current->operation[i].a.max << ", "
				<< current->operation[i].b.min << ", "
				<< current->operation[i].b.max;
		}
		else if (length == 5) {
			if (variable[4] == 'a') {
				oss << current->operation[i].a.min << ", "
					<< current->operation[i].a.max;
			}
			else {
				oss << current->operation[i].b.min << ", "
					<< current->operation[i].b.max;
			}
		}
		else {
			if (variable[4] == 'a') {
				if (variable[7] == 'i') {
					oss << current->operation[i].a.min;
				}
				else {
					oss << current->operation[i].a.max;
				}
			}
			else if (variable[4] == 'b') {
				if (variable[7] == 'i') {
					oss << current->operation[i].b.min;
				}
				else {
					oss << current->operation[i].b.max;
				}
			}
			else {
				oss << current->operation[i]._switch;
			}
		}
		};
	switch (variable[0]) {
	case 's':
		if (variable[1] == 'c') {
			oss << current->score;
		}
		else {
			switch_operation(1);
		}
		break;
	case 'b':
		if (length == 5) {
			oss << current->bonus.time << ", "
				<< current->bonus.range;
		}
		else {
			if (variable[6] == 't') {
				oss << current->bonus.time;
			}
			else {
				oss << current->bonus.range;
			}
		}
		break;
	case 'a':
		switch_operation(0);
		break;
	case 'm':
		switch_operation(2);
		break;
	case 'd':
		switch_operation(3);
		break;
	case 'q':
		oss << current->quantity;
		break;
	case 'p':
		oss << password;
		break;
	default:
		break;
	}
	feedback = "��ǰֵΪ��" + oss.str();
}
void ArithAdmin::renew_password() {
	std::ofstream ofs(".\\data\\password.bin", std::ios::binary);
	if (ofs.is_open()) {
		std::string original = Graph_IO::input;
		Arithmetic::encrypt(original);
		ofs.write(original.data(), original.size());
		ofs.close();
	}
	current->save();
}
void ArithAdmin::check_value(clock_t delta) {
	out = "���������";
	if (!Graph_IO::input.empty()) {
		if (!checked && variable != "password") {
			mistaken = 0;
			for (const char c : Graph_IO::input) {
				if (!(c >= '0' && c <= '9' || c == ' ')) {
					mistaken = 1;
					break;
				}
			}
			checked = 1;
		}
		if(!mistaken){
			std::istringstream iss(Graph_IO::input);
			size_t length = variable.length();
			auto switch_operation = [&](const size_t i) {
				if (length == 3) {
					iss >> current->operation[i]._switch
						>> current->operation[i].a.min
						>> current->operation[i].a.max
						>> current->operation[i].b.min
						>> current->operation[i].b.max;
				}
				else if (length == 5) {
					if (variable[4] == 'a') {
						iss >> current->operation[i].a.min
							>> current->operation[i].a.max;
					}
					else {
						iss >> current->operation[i].b.min
							>> current->operation[i].b.max;
					}
				}
				else {
					if (variable[4] == 'a') {
						if (variable[7] == 'i') {
							iss >> current->operation[i].a.min;
						}
						else {
							iss >> current->operation[i].a.max;
						}
					}
					else if (variable[4] == 'b') {
						if (variable[7] == 'i') {
							iss >> current->operation[i].b.min;
						}
						else {
							iss >> current->operation[i].b.max;
						}
					}
					else {
						iss >> current->operation[i]._switch;
					}
				}
				};
			switch (variable[0]) {
			case 's':
				if (variable[1] == 'c') {
					iss >> current->score;
				}
				else {
					switch_operation(1);
				}
				break;
			case 'b':
				if (length == 5) {
					iss >> current->bonus.time >> current->bonus.range;
				}
				else {
					if (variable[6] == 't') {
						iss >> current->bonus.time;
					}
					else {
						iss >> current->bonus.range;
					}
				}
				break;
			case 'a':
				switch_operation(0);
				break;
			case 'm':
				switch_operation(2);
				break;
			case 'd':
				switch_operation(3);
				break;
			case 'q':
				iss >> current->quantity;
				break;
			case 'p':
				renew_password();
				break;
			default:
				break;
			}
			if (proceed_loading(feedback, delta, 5, "�����룬���ڱ��档")) {
				current->save();
				if (variable == "password") {
					Graph_IO::input = "/invisible/";
				}
				current->log(variable + "->" + Graph_IO::input);
				Graph_IO::input.clear();
				state = 8;
				checked = 0;
				if (!greyified) {
					greyify_blur_screen();
					greyified = 1;
				}
			}
		}
		else {
			if (proceed_loading(feedback, delta, 5, "�����з����ֻ�ո��ַ���")) {
				state = 6;
				checked = 0;
				Graph_IO::input.clear();
			}
		}
	}
	else {
		mistaken = 1;
		if (proceed_loading(feedback, delta, 5, "����ֵ����Ϊ�ա�")) {
			checked = 0;
			state = 6;
			Graph_IO::input.clear();
		}
	}
}
void ArithAdmin::ask_back() {
	out = "�������û������ѡ��U/V��?";
}
void ArithAdmin::back_user(clock_t delta) {
	if (proceed_loading(out, delta, 5, "���ڷ����û�ѡ����档")) {
		state = 2;
	}
}
void ArithAdmin::back_variable(clock_t delta) {
	if (proceed_loading(out, delta, 5, "���ڷ��ر���ѡ����档")) {
		state = 4;
	}
}
void ArithAdmin::ask_quit() {
	out = "�Ƿ񷵻ص�¼���桾Y/N��?";
	if (!greyified) {
		greyify_blur_screen();
		greyified = 1;
	}
}
void ArithAdmin::quit(clock_t delta) {
	if (proceed_loading(out, delta, 5, "���ڷ��ص�¼���档")) {
		current_mode = "kousuan";
		ended = 1;
	}
}