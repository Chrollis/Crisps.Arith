#include "arith_admin.h"

void ArithAdmin::enter() {
	enter_loading(5, "管理员界面加载中。");
}
void ArithAdmin::draw() {
	Graph_IO::set_text_style(L"霞鹜漫黑");
	setbkmode(TRANSPARENT);
	setlinestyle(PS_SOLID, 1);
	if (state == InputPassword ||
		state == CheckPassword ||
		state == InputID ||
		state == CheckID ||
		state == InputVariable ||
		state == CheckVariable ||
		state == InputValue ||
		state == CheckValue) {
		Graph_IO::output_line(DARKGRAY, input_line_position);
		Graph_IO::draw_text(BLACK, out, &question_rect,
			DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		if (state != InputPassword &&
			state != CheckPassword) {
			Graph_IO::draw_text(MIDNIGHTBLUE, Graph_IO::input + "<-", &input_rect,
				DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
		}
		else {
			std::string str(Graph_IO::input.length(), '*');
			Graph_IO::draw_text(MIDNIGHTBLUE, str + "<-", &input_rect,
				DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
		}
		if (state == CheckPassword || 
			state == CheckID || 
			state == CheckVariable || 
			state == CheckValue) {
			Graph_IO::draw_text(LIMEGREEN, feedback, &feedback_rect,
				DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		}
		else if (state == InputValue) {
			Graph_IO::draw_text(LIMEGREEN, feedback, &feedback_rect,
				DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		}
		else if (check_mistaken) {
			Graph_IO::draw_text(FIREBRICK3, feedback, &feedback_rect,
				DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		}
	}
	else {
		pop_up_draw();
	}
}
void ArithAdmin::timekeep(clock_t delta) {
	la.timekeep(delta);
}
void ArithAdmin::proceed() {
	switch (state)
	{
	case InputPassword:
		input_password();
		break;
	case CheckPassword:
		check_password();
		break;
	case InputID:
		input_id();
		break;
	case CheckID:
		check_id();
		break;
	case InputVariable:
		input_variable();
		break;
	case CheckVariable:
		check_variable();
		break;
	case InputValue:
		input_value();
		break;
	case CheckValue:
		check_value();
		break;
	case AskBack:
		ask_back();
		break;
	case BackUser:
		back_user();
		break;
	case BackVariable:
		back_variable();
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
void ArithAdmin::input(const ExMessage& msg) {
	if (state == InputPassword) {
		Graph_IO::input_text(msg);
		if (msg.message == WM_KEYDOWN && 
			(msg.vkcode == VK_RETURN ||
				msg.vkcode == VK_SEPARATOR)) {
			state = CheckPassword;
			Graph_IO::input.pop_back();
		}
	}else if (state == InputID) {
		Graph_IO::input_text(msg);
		if (msg.message == WM_KEYDOWN &&
			(msg.vkcode == VK_RETURN ||
				msg.vkcode == VK_SEPARATOR)) {
			state = CheckID;
			Graph_IO::input.pop_back();
		}
	}
	else if (state == InputVariable) {
		Graph_IO::input_text(msg);
		if (msg.message == WM_KEYDOWN &&
			(msg.vkcode == VK_RETURN ||
				msg.vkcode == VK_SEPARATOR)) {
			state = CheckVariable;
			Graph_IO::input.pop_back();
		}
	}
	else if (state == InputValue) {
		Graph_IO::input_text(msg);
		if (msg.message == WM_KEYDOWN &&
			(msg.vkcode == VK_RETURN ||
				msg.vkcode == VK_SEPARATOR)) {
			state = CheckValue;
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
				if (state != AskBack) {
					screenshotted = 0;
				}
			}
		}
		else if (state == AskBack) {
			if (msg.vkcode == 'U') {
				state = BackUser;
				screenshotted = 0;
			}
			else if (msg.vkcode == 'V') {
				state = BackVariable;
				screenshotted = 0;
			}
			else if(msg.vkcode == VK_ESCAPE) {
				last_state = state;
				state = AskQuit;
			}
		}
		else if (msg.vkcode == VK_ESCAPE) {
			last_state = state;
			state = AskQuit;
		}
	}
}
void ArithAdmin::exit() {
	delete current;
	Graph_IO::input.clear();
}

void ArithAdmin::input_password() {
	out = "请输入密码：";
}
void ArithAdmin::check_password() {
	out = "输入结束。";
	std::string password;
	Arithmetic::get_password(password);
	if (Graph_IO::input == password) {
		if (la.proceed_loading(feedback, 5, "密码正确，正在重定向。")) {
			state = InputID;
			la.reset();
			Graph_IO::input.clear();
		}
	}
	else {
		if (la.proceed_loading(feedback, 5, "密码错误，程序终止。")) {
			current_mode = "exit";
			current_ended = 1;
		}
	}
}
void ArithAdmin::input_id() {
	out = "请输入目标ID：";
}
void ArithAdmin::check_id() {
	out = "输入结束。";
	if (!checked) {
		std::regex rgx("[a-zA-Z0-9_]{1,10}");
		check_mistaken = !std::regex_match(Graph_IO::input, rgx);
		checked = 1;
	}
	if (check_mistaken) {
		if (la.proceed_loading(feedback)) {
			state = InputID;
			la.reset();
			Graph_IO::input.clear();
			feedback = "ID非法，请重新输入。";
			checked = 0;
		}
	}
	else {
		if (Graph_IO::input != "admin") {
			if (la.proceed_loading(feedback, 5, "该用户名合法，正在加载。")) {
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
				state = InputVariable;
				la.reset();
				checked = 0;
			}
		}
		else {
			if (la.proceed_loading(feedback)) {
				check_mistaken = 1;
				state = InputID;
				la.reset();
				Graph_IO::input.clear();
				feedback = "请勿重复输入管理员ID。";
				checked = 0;
			}
		}
	}
}
void ArithAdmin::input_variable() {
	out = "请输入变量名称：";
}
void ArithAdmin::check_variable() {
	out = "输入结束。";
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
		check_mistaken = 1;
		for (const std::string& var : legal_varible) {
			if (Graph_IO::input == var) {
				check_mistaken = 0;
				break;
			}
		}
		checked = 1;
	}
	if (!check_mistaken) {
		if (la.proceed_loading(feedback, 5, "该变量存在，正在跳转。")) {
			variable = Graph_IO::input;
			Graph_IO::input.clear();
			checked = 0;
			state = InputValue;
			la.reset();
		}
	}
	else {
		if (la.proceed_loading(feedback)) {
			state = InputVariable;
			la.reset();
			checked = 0;
			Graph_IO::input.clear();
			feedback = "该变量不存在，请重新输入。";
		}
	}
}
void ArithAdmin::input_value() {
	out = "请输入目标值：";
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
	feedback = "当前值为：" + oss.str();
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
void ArithAdmin::check_value() {
	out = "输入结束。";
	if (!checked && variable != "password") {
		std::regex rgx("((0|([1-9][0-9]*))\\s*)+");
		check_mistaken = !std::regex_match(Graph_IO::input, rgx);
		checked = 1;
	}
	if (check_mistaken) {
		if (la.proceed_loading(feedback, 5, "输入有非数字或空格字符。")) {
			state = InputValue;
			la.reset();
			checked = 0;
			Graph_IO::input.clear();
		}
	}
	else {
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
		if (la.proceed_loading(feedback, 5, "已输入，正在保存。")) {
			current->save();
			if (variable == "password") {
				Graph_IO::input = "/invisible/";
			}
			current->log(variable + "->" + Graph_IO::input);
			Graph_IO::input.clear();
			state = AskBack;
			la.reset();
			checked = 0;
			if (!screenshotted) {
				greyify_blur_screen();
				screenshotted = 1;
			}
		}
	}
}
void ArithAdmin::ask_back() {
	out = "返回至用户或变量选择【U/V】?";
}
void ArithAdmin::back_user() {
	if (la.proceed_loading(out, 5, "正在返回用户选择界面。")) {
		state = InputID;
		la.reset();
	}
}
void ArithAdmin::back_variable() {
	if (la.proceed_loading(out, 5, "正在返回变量选择界面。")) {
		state = InputVariable;
		la.reset();
	}
}
void ArithAdmin::ask_quit() {
	out = "是否返回登录界面【Y/N】?";
	if (!screenshotted) {
		greyify_blur_screen();
		screenshotted = 1;
	}
}
void ArithAdmin::quit() {
	if (la.proceed_loading(out, 5, "正在返回登录界面。")) {
		current_mode = "kousuan";
		current_ended = 1;
	}
}