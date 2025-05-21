#include "points_24.h"

void Points24::enter() {
	enter_loading(5, "24点加载中。");
}
void Points24::draw() {
	Graph_IO::set_text_style(L"霞鹜漫黑");
	setbkmode(TRANSPARENT);
	setlinestyle(PS_SOLID, 1);

	if (state == InputNum ||
		state == Compute) {
		normal_draw();
		if (state == Compute && !input_check_mistaken) {
			Graph_IO::draw_text(LIMEGREEN, feedback, &feedback_rect,
				DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		}
		else if (state == InputNum) {
			Graph_IO::draw_text(DARKVIOLET, feedback, &feedback_rect,
				DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			Graph_IO::output_text(DARKVIOLET, feedback_rect.left, feedback_rect.bottom, feedbackA);
		}
		else if (input_check_mistaken) {
			Graph_IO::draw_text(FIREBRICK3, feedback, &feedback_rect,
				DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		}
	}
	else {
		pop_up_draw();
	}
}
void Points24::timekeep(clock_t delta) {
	loading += delta;
}
void Points24::proceed() {
    switch (state) {
    case InputNum:
        input_num();
        break;
    case Compute:
        compute();
        break;
    case AskRegenerate:
        ask_regenerate();
        break;
	case ConfirmSolvable :
		confirm_solvable();
		break;
	case ConfirmRestart:
		confirm_restart();
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
void Points24::input(const ExMessage& msg) {
	if (state == InputNum) {
		Graph_IO::input_text(msg);
		if (msg.message == WM_KEYDOWN &&
			(msg.vkcode == VK_RETURN ||
				msg.vkcode == VK_SEPARATOR)) {
			state = Compute;
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
		else if (state == AskRegenerate) {
			if (msg.vkcode == 'Y' ||
				msg.vkcode == VK_RETURN || msg.vkcode == VK_SEPARATOR) {
				state = InputNum;
				num_generated = 0;
			}
			else if (msg.vkcode == 'N' || msg.vkcode == VK_ESCAPE) {
				state = Quit;
				screenshotted = 0;
			}
		}
		else if (state == ConfirmSolvable) {
			if (msg.vkcode == 'Y' ||
				msg.vkcode == VK_RETURN ||
				msg.vkcode == VK_SEPARATOR) {
				if (!solvable) {
					state = AskRegenerate;
					screenshotted = 0;
					num_generated = 0;
					solvable_checked = 0;
				}
				else {
					solvable_checked = 1;
				}
			}
			else if (msg.vkcode == 'N' ||
				msg.vkcode == VK_ESCAPE) {
				state = InputNum;
				screenshotted = 0;
				solvable_checked = 0;
			}
		}
		else if (state == ConfirmRestart) {
			if (msg.vkcode == 'Y' ||
				msg.vkcode == VK_RETURN ||
				msg.vkcode == VK_SEPARATOR) {
				nums.clear();
				for (int i = 0; i < 4; i++) {
					nums.push_back(std::pair<char, double>('a' + i, numbers[i]));
				}
			}
			else if (msg.vkcode == 'N' ||
				msg.vkcode == VK_ESCAPE) {
				screenshotted = 0;
			}
			state = InputNum;
		}
		else if (msg.vkcode == VK_ESCAPE) {
			last_state = state;
			state = AskQuit;
		}
	}
}
void Points24::exit() {
	Graph_IO::input.clear();
}

void Points24::generate_num() {
	numbers.clear();
	nums.clear();
	for (int i = 0; i < 4; i++) {
		numbers.push_back(int_rand(1, 13));
		nums.push_back(std::pair<char, double>('a' + i, numbers[i]));
	}
	solvable = solve_24(numbers, solutions);
}
void Points24::input_num() {
	if (!num_generated) {
		generate_num();
		num_generated = 1;
	}
	out = "使用" + std::to_string(numbers[0]) + ", " +
		std::to_string(numbers[1]) + ", " +
		std::to_string(numbers[2]) + ", " +
		std::to_string(numbers[3]) + "计算24：";
	feedback.clear();
	feedbackA.clear();
	int s = int(nums.size());
	for (int i = 0; i < s; i++) {
		if (i < 2) {
			feedback += nums[i].first;
			std::string num = std::to_string(nums[i].second);
			feedback += " = " + num.
				substr(0, num.find('.') + 3);
			feedback += ", ";
		}
		else {
			feedbackA += nums[i].first;
			std::string num = std::to_string(nums[i].second);
			feedbackA += " = " + num.
				substr(0, num.find('.') + 3);
			feedbackA += ", ";
		}
	}
	feedback += "uns->无解";
	feedbackA += "res->重来";
}
void Points24::compute() {
	out = "输入结束。";
	if (Graph_IO::input == "uns") {
		state = ConfirmSolvable;
		Graph_IO::input.clear();
		if (!screenshotted) {
			greyify_blur_screen();
			screenshotted = 1;
		}
	}
	else if (Graph_IO::input == "res") {
		state = ConfirmRestart;
		Graph_IO::input.clear();
		if (!screenshotted) {
			greyify_blur_screen();
			screenshotted = 1;
		}
	}
	else {
		std::string A, op, B;
		std::smatch sm;
		if (!input_checked) {
			std::regex letter_rgx("^([a-d])([+\\-*/])(?!\\1)([a-d])$");
			std::regex digit_rgx("^((?:0|[1-9]\\d*)(?:\\.\\d{2}))([+\\-*/])((?:0|[1-9]\\d*)(?:\\.\\d{2}))$");
			if (std::regex_match(Graph_IO::input, sm, letter_rgx)) {
				A = sm[1];
				op = sm[2];
				B = sm[3];
				letter_or_digit = Letter;
			}
			else if (std::regex_match(Graph_IO::input, sm, digit_rgx)) {
				A = sm[1];
				op = sm[2];
				B = sm[3];
				letter_or_digit = Digit;
			}
			else {
				input_check_mistaken = 1;
			}
			input_checked = 1;
		}
		if (input_check_mistaken) {
			if (loading.proceed_loading(feedback, 5, "表达式非法，请重新输入。")) {
				state = InputNum;
				input_checked = 0;
				loading(0);
				Graph_IO::input.clear();
			}
		}
		else {
			std::pair<char, double>* a = 0, * b = 0;
			if (letter_or_digit == Letter) {
				for (std::pair<char, double>& num : nums) {
					if (A[0] == num.first) {
						a = &num;
						break;
					}
				}
				for (std::pair<char, double>& num : nums) {
					if (B[0] == num.first) {
						b = &num;
						break;
					}
				}
			}
			else {
				for (std::pair<char, double>& num : nums) {
					std::string numstr = std::to_string(num.second);
					if (A == numstr.substr(0, numstr.find('.') + 3)) {
						a = &num;
						break;
					}
				}
				for (std::pair<char, double>& num : nums) {
					std::string numstr = std::to_string(num.second);
					if (B == numstr.substr(0, numstr.find('.') + 3)) {
						b = &num;
						if (a != b) {
							break;
						}
						else {
							b = 0;
						}
					}
				}
			}
			input_check_mistaken = !a || !b;
			if (!input_check_mistaken) {
				double result = 0;
				if (calculate(a->second, b->second, op[0], result)) {
					a->second = result;
					std::swap(nums.back(), *b);
					nums.pop_back();
					input_checked = 0;
					std::sort(nums.begin(), nums.end(),
						[](std::pair<char, double> a, std::pair<char, double> b) {
							return a.first < b.first;
						});
					Graph_IO::input.clear();
					if (nums.size() == 1) {
						if (abs(result - 24) < EPSILON) {
							num_generated = 0;
							state = AskRegenerate;
							if (!screenshotted) {
								greyify_blur_screen();
								screenshotted = 1;
							}
						}
					}
					else {
						state = InputNum;
					}
				}
				else {
					input_check_mistaken = 1;
					if (loading.proceed_loading(feedback, 5, "除数为零，请重新输入。")) {
						state = InputNum;
						loading(0);
						Graph_IO::input.clear();
						input_checked = 0;
					}
				}
			}
			else {
				if (loading.proceed_loading(feedback, 5, "所输入数字或字母不可用。")) {
					state = InputNum;
					loading(0);
					Graph_IO::input.clear();
					input_checked = 0;
				}
			}
		}
	}
}
void Points24::ask_regenerate() {
	out = "24点求解成功，是否再来【Y/N】?";
}
void Points24::confirm_solvable() {
	if (!solvable_checked) {
		out = "是否确认无解【Y/N】?";
	}
	else {
		if (loading.proceed_loading(out, 5, "其实有解，更换题目中。")) {
			solvable_checked = 0;
			num_generated = 0;
			screenshotted = 0;
			state = InputNum;
			loading(0);
		}
	}
}
void Points24::confirm_restart() {
		out = "是否确认重来【Y/N】?";
}
void Points24::ask_quit() {
	out = "是否返回主菜单【Y/N】?";
	if (!screenshotted) {
		greyify_blur_screen();
		screenshotted = 1;
	}
}
void Points24::quit() {
	if (loading.proceed_loading(out, 5, "正在返回主菜单。")) {
		current_mode = "menu";
		current_ended = 1;
	}
}

bool Points24::calculate(double a, double b, char op, double& result) {
	switch (op) {
	case '+':
		result = a + b;
		return true;
	case '-':
		result = a - b;
		return true;
	case '*':
		result = a * b;
		return true;
	case '/':
		if (fabs(b) < EPSILON) {
			return false;
		}
		result = a / b;
		return true;
	default:
		return false;
	}
}
void Points24::generate_permutations(std::vector<int>& nums, int start, std::vector<std::vector<int>>& result) {
	if (start == nums.size()) {
		result.push_back(nums);
		return;
	}
	std::unordered_set<int> seen;
	for (int i = start; i < nums.size(); ++i) {
		if (seen.count(nums[i])) continue;
		seen.insert(nums[i]);
		std::swap(nums[start], nums[i]);
		generate_permutations(nums, start + 1, result);
		std::swap(nums[start], nums[i]);
	}
}
std::vector<std::vector<int>> Points24::generate_all_permutations(std::vector<int>& nums) {
	std::vector<std::vector<int>> result;
	generate_permutations(nums, 0, result);
	return result;
}
bool Points24::solve_24(std::vector<int>& nums, std::set<std::string>& solutions) {
	solutions.clear();
	std::vector<std::vector<int>> permutations = generate_all_permutations(nums);
	std::vector<char> operators = { '+', '-', '*', '/' };
	bool found = false;

    for (const auto& perm : permutations) {
        for (char op1 : operators) {
            for (char op2 : operators) {
                for (char op3 : operators) {
                    double temp1, temp2, temp3;
                    bool valid;
                    std::string expr;

                    // Structure 1: ((a op1 b) op2 c) op3 d
                    valid = true;
                    valid &= calculate(perm[0], perm[1], op1, temp1);
                    if (valid) valid &= calculate(temp1, perm[2], op2, temp2);
                    if (valid) valid &= calculate(temp2, perm[3], op3, temp3);
                    if (valid && fabs(temp3 - 24) < EPSILON) {
                        expr = "((" + std::to_string(perm[0]) + " " + op1 + " " + std::to_string(perm[1]) + ") " + op2 + " " + std::to_string(perm[2]) + ") " + op3 + " " + std::to_string(perm[3]);
                        solutions.insert(expr);
                        found = true;
                    }

                    // Structure 2: (a op1 (b op2 c)) op3 d
                    valid = true;
                    valid &= calculate(perm[1], perm[2], op2, temp1);
                    if (valid) valid &= calculate(perm[0], temp1, op1, temp2);
                    if (valid) valid &= calculate(temp2, perm[3], op3, temp3);
                    if (valid && fabs(temp3 - 24) < EPSILON) {
                        expr = "(" + std::to_string(perm[0]) + " " + op1 + " (" + std::to_string(perm[1]) + " " + op2 + " " + std::to_string(perm[2]) + ")) " + op3 + " " + std::to_string(perm[3]);
                        solutions.insert(expr);
                        found = true;
                    }

                    // Structure 3: a op1 ((b op2 c) op3 d)
                    valid = true;
                    valid &= calculate(perm[1], perm[2], op2, temp1);
                    if (valid) valid &= calculate(temp1, perm[3], op3, temp2);
                    if (valid) valid &= calculate(perm[0], temp2, op1, temp3);
                    if (valid && fabs(temp3 - 24) < EPSILON) {
                        expr = std::to_string(perm[0]) + " " + op1 + " ((" + std::to_string(perm[1]) + " " + op2 + " " + std::to_string(perm[2]) + ") " + op3 + " " + std::to_string(perm[3]) + ")";
                        solutions.insert(expr);
                        found = true;
                    }

                    // Structure 4: a op1 (b op2 (c op3 d))
                    valid = true;
                    valid &= calculate(perm[2], perm[3], op3, temp1);
                    if (valid) valid &= calculate(perm[1], temp1, op2, temp2);
                    if (valid) valid &= calculate(perm[0], temp2, op1, temp3);
                    if (valid && fabs(temp3 - 24) < EPSILON) {
                        expr = std::to_string(perm[0]) + " " + op1 + " (" + std::to_string(perm[1]) + " " + op2 + " (" + std::to_string(perm[2]) + " " + op3 + " " + std::to_string(perm[3]) + "))";
                        solutions.insert(expr);
                        found = true;
                    }

                    // Structure 5: (a op1 b) op2 (c op3 d)
                    valid = true;
                    double temp4;
                    valid &= calculate(perm[0], perm[1], op1, temp1);
                    valid &= calculate(perm[2], perm[3], op3, temp4);
                    if (valid) valid &= calculate(temp1, temp4, op2, temp3);
                    if (valid && fabs(temp3 - 24) < EPSILON) {
                        expr = "(" + std::to_string(perm[0]) + " " + op1 + " " + std::to_string(perm[1]) + ") " + op2 + " (" + std::to_string(perm[2]) + " " + op3 + " " + std::to_string(perm[3]) + ")";
                        solutions.insert(expr);
                        found = true;
                    }
                }
            }
        }
    }
    return found;
}