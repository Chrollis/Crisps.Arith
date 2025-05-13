#include "arithmetic.h"

void Arithmetic::enter() {
	enter_loading(3, "用户数据加载中。");
	if (read()) {
		enter_loading(2, "用户数据加载成功。");
		log("started");
	}
	else {
		enter_loading(2, "用户数据不存在。");
		enter_loading(3, "用户数据创建中。");
		save();
		enter_loading(2, "用户数据创建成功。");
		log("created");
	}
}
void Arithmetic::draw() {
	Graph_IO::set_text_style(L"霞鹜漫黑");
	setbkmode(TRANSPARENT);
	setlinestyle(PS_SOLID, 1);

	if (state == InputAnswer ||
		state == CheckAnswer) {
		normal_draw();
		Graph_IO::draw_text(DARKVIOLET, show_score, &question_rect,
			DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		if (state == InputAnswer && !answer_check_mistaken) {
			Graph_IO::draw_text(LIMEGREEN, feedback, &feedback_rect,
				DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		}
		else if (answer_check_mistaken) {
			Graph_IO::draw_text(FIREBRICK3, feedback, &feedback_rect,
				DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		}
		else {
			if (last_wrong) {
				Graph_IO::draw_text(FIREBRICK3, feedback, &feedback_rect,
					DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
			}
			else {
				Graph_IO::draw_text(LIMEGREEN, feedback, &feedback_rect,
					DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
			}
		}
	}
	else if (state == SettleScore) {
		Graph_IO::draw_text(BLACK, out, &question_rect, 
			DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		Graph_IO::draw_text(LIMEGREEN, feedback, &input_rect,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		Graph_IO::draw_text(MIDNIGHTBLUE, show_score, &feedback_rect,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else {
		pop_up_draw();
	}
}
void Arithmetic::timekeep(clock_t delta) {
	la.timekeep(delta);
	if (thinking) {
		thinking_timer += delta;
	}
}
void Arithmetic::proceed() {
	show_score = "当前" + std::to_string(score) + "分。";
	switch (state) {
	case InputAnswer:
		input_answer();
		break;
	case CheckAnswer:
		check_answer();
		break;
	case SettleScore:
		settle_score();
		break;
	case SaveScore:
		save_score();
		break;
	case AskNewTurn:
		ask_new_turn();
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
void Arithmetic::input(const ExMessage& msg) {
	if (state == InputAnswer) {
		Graph_IO::input_text(msg);
		if (msg.message == WM_KEYDOWN &&
			(msg.vkcode == VK_RETURN ||
				msg.vkcode == VK_SEPARATOR)) {
			state = CheckAnswer;
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
		else if (state == AskNewTurn) {
			if (msg.vkcode == 'N' ||
				msg.vkcode == VK_ESCAPE) {
				state = Quit;
			}
			else if (msg.vkcode == 'Y' ||
				msg.vkcode == VK_RETURN ||
				msg.vkcode == VK_SEPARATOR) {
				*this = Arithmetic();
				this->read();
			}
		}
		else if (state == SettleScore) {
			if (msg.vkcode == 'Y' ||
				msg.vkcode == VK_RETURN ||
				msg.vkcode == VK_SEPARATOR) {
				save();
				whether_to_save = 1;
				log("saved=" + std::to_string(score));
				state = SaveScore;
			}
			else if (msg.vkcode == 'N' ||
				msg.vkcode == VK_ESCAPE) {
				whether_to_save = 0;
				log("quit to save");
				state = SaveScore;
			}
		}
		else if (msg.vkcode == VK_ESCAPE) {
			last_state = state;
			state = AskQuit;
		}
	}
}
void Arithmetic::exit() {
	Graph_IO::input.clear();
}

void Arithmetic::generate_question() {
	std::vector<std::string> operation_signal = { "+","-", "×","÷" };
	int op_index;
	int a, b;
	do {
		do {
			op_index = int_rand(0, 3);
		} while (!operation[op_index]._switch);
		Operation& current_op = operation[op_index];
		
		if (op_index == 3) {
			a = int_rand(current_op.a.min, current_op.a.max);
			do {
				b = int_rand(current_op.b.min, current_op.b.max);
			} while (b == 0 || a < b);
		}
		else if (op_index == 1) {
			do {
				a = int_rand(current_op.a.min, current_op.a.max);
				b = int_rand(current_op.b.min, current_op.b.max);
			} while (a < b);
		}
		else {
			a = int_rand(current_op.a.min, current_op.a.max);
			b = int_rand(current_op.b.min, current_op.b.max);
		}
		if (op_index == 3) {
			answer = std::to_string(a / b) + "..." + std::to_string(a % b);
		}
		else {
			if (op_index == 0) {
				answer = std::to_string(a + b);
			}
			else if (op_index == 1) {
				answer = std::to_string(a - b);
			}
			else if (op_index == 2) {
				answer = std::to_string(a * b);
			}
		}
		question = std::to_string(a) + operation_signal[op_index] + std::to_string(b) + '=';
	} while (question == last_question);
	last_question = question;
}
void Arithmetic::input_answer() {
	if (!question_generated && !last_wrong) {
		generate_question();
		question_generated = 1;
		thinking = 1;
	}
	out = question;
}
void Arithmetic::check_answer() {
	if (!answer_checked) {
		std::regex rgx("(-?(0|([1-9][0-9]*)))(\\.{3}(0|([1-9][0-9]*)))?");
		answer_check_mistaken = !std::regex_match(Graph_IO::input, rgx);
	}
	if (answer_check_mistaken) {
		feedback = "非法输入，请重新输入。";
		state = InputAnswer;
		answer_checked = 0;
		Graph_IO::input.clear();
	}
	else {
		if (Graph_IO::input == answer) {
			last_wrong = 0;
			feedback = "回答正确。";
			score += max(0, (int)(bonus.range * (1 - (double)thinking_timer / bonus.time)));
			thinking = 0;
			thinking_timer = 0;
			question_count += 1;
			question_generated = 0;
		}
		else {
			last_wrong = 1;
			feedback = "回答错误。";
			score -= bonus.range;
			wrong_count += 1;
		}
		if (question_count == quantity) {
			state = SettleScore;
		}
		else {
			state = InputAnswer;
		}
		Graph_IO::input.clear();
	}
}
void Arithmetic::settle_score() {
	if (!bonused) {
		int wrong_rate = (int)((double)wrong_count * 10000 / (quantity + wrong_count));
		out = "答题结束，错误率为" + std::to_string(wrong_rate / 100) + '.' + std::to_string(wrong_rate % 100) + "%。";
		int bonus_score = quantity - wrong_rate / (10000 / quantity);
		score += bonus_score;
		feedback = "奖励" + std::to_string(bonus_score) + "分，是否保存【Y/N】？";
		bonused = 1;
	}
}
void Arithmetic::save_score() {
	if (!screenshotted) {
		greyify_blur_screen();
		screenshotted = 1;
	}
	if (whether_to_save) {
		if (la.proceed_loading(out, 5, "正在保存，请稍后。")) {
			screenshotted = 0;
			state = AskNewTurn;
			la.reset();
		}
	}
	else {
		if (la.proceed_loading(out, 5, "已放弃保存，请稍后。")) {
			screenshotted = 0;
			state = AskNewTurn;
			la.reset();
		}
	}
}
void Arithmetic::ask_new_turn() {
	out = "是否创建新题目【Y/N】?";
}
void Arithmetic::ask_quit() {
	out = "是否返回主菜单【Y/N】?";
	if (!screenshotted) {
		greyify_blur_screen();
		screenshotted = 1;
	}
}
void Arithmetic::quit() {
	if (la.proceed_loading(out, 5, "正在返回主菜单。")) {
		current_mode = "menu";
		current_ended = 1;
	}
}

void Arithmetic::encrypt(std::string& input, const std::string& key) {
	size_t length = key.length();
	size_t end = input.length();
	size_t key_index = 0;
	for (size_t i = 0; i < end; i++) {
		input[i] ^= key[key_index++];
		key_index %= length;
	}
}
bool Arithmetic::get_password(std::string& password) {
	std::ifstream ifs(".\\data\\password.bin", std::ios::binary);
	std::string original;
	if (!ifs.is_open()) {
		std::ofstream ofs(".\\data\\password.bin", std::ios::binary);
		if (!ofs.is_open()) {
			return 0;
		}
		password = "password";
		original = password;
		encrypt(original);
		ofs.write(original.data(), original.size());
		ofs.close();
	}
	else {
		original = std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		ifs.close();
		encrypt(original);
		std::istringstream iss(original);
		iss >> password;
	}
	return 1;
}
bool Arithmetic::log(std::string out_str) {
	std::ofstream ofs(".\\data\\log.txt", std::ios::app);
	if (!ofs.is_open()) {
		return 0;
	}
	ofs << get_time_string() << ": ";
	ofs << "ID: " << current_id << "; ";
	ofs << "Issue: " << out_str << std::endl;
	return 1;
}
bool Arithmetic::save() const {
	std::ostringstream oss;
	oss << score << ' ' << quantity << ' ' << bonus.time << ' '
		<< bonus.range << ' ';
	for (int i = 0; i < 4; ++i) {
		oss << operation[i]._switch << ' '
			<< operation[i].a.min << ' ' << operation[i].a.max << ' '
			<< operation[i].b.min << ' ' << operation[i].b.max << ' ';
	}
	std::string original = oss.str();
	std::string password;
	Arithmetic::get_password(password);
	encrypt(original, password);
	std::ofstream ofs(".\\data\\" + current_id + ".bin", std::ios::binary);
	if (!ofs.is_open()) {
		return 0;
	}
	ofs.write(original.data(), original.size());
	ofs.close();
	return 1;
}
bool Arithmetic::read() {
	std::ifstream ifs(".\\data\\" + current_id + ".bin", std::ios::binary);
	if (!ifs.is_open()) {
		return 0;
	}
	std::string original((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	ifs.close();
	std::string password;
	Arithmetic::get_password(password);
	encrypt(original, password);
	std::istringstream iss(original);
	iss >> score >> quantity >> bonus.time
		>> bonus.range;
	for (int i = 0; i < 4; ++i) {
		iss >> operation[i]._switch
			>> operation[i].a.min >> operation[i].a.max
			>> operation[i].b.min >> operation[i].b.max;
	}
	return 1;
}