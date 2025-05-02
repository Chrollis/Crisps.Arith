#include "points_24.h"

void Points24::enter() {
	enter_loading(5, "24点加载中。");
}
void Points24::draw() {
	Graph_IO::set_text_style(L"霞鹜漫黑");
	setbkmode(TRANSPARENT);
	setlinestyle(PS_SOLID, 1);

	if (state < 2) {
		normal_draw();
		if (state == 1 && !mistaken) {
			Graph_IO::draw_text(LIMEGREEN, feedback, &feedback_rect,
				DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		}
		else if (state == 0) {
			Graph_IO::draw_text(DARKVIOLET, feedback, &feedback_rect,
				DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			Graph_IO::output_text(DARKVIOLET, feedback_rect.left, feedback_rect.bottom, fedback);
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
void Points24::proceed(clock_t delta) {
    switch (state) {
    case 0:
        generate_num();
        break;
    case 1:
        compute(delta);
        break;
    case 2:
        ask_regenerate();
        break;
	case 3 :
		confirm_solvable(delta);
		break;
	case 4:
		confirm_restart();
		break;
    case 5:
        ask_quit();
        break;
    case 6:
        quit(delta);
        break;
    default:
        break;
    }
}
void Points24::input(const ExMessage& msg) {
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
		if (state == 5) {
			if (msg.vkcode == 'Y' ||
				msg.vkcode == VK_RETURN || msg.vkcode == VK_SEPARATOR) {
				state = 6;
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
				state = 6;
				greyified = 0;
			}
		}
		else if (state == 3) {
			if (msg.vkcode == 'Y' ||
				msg.vkcode == VK_RETURN || msg.vkcode == VK_SEPARATOR) {
				if (!solvable) {
					state = 2;
					greyified = 0;
					generated = 0;
					checked = 0;
				}
				else {
					checked = 1;
				}
			}
			else if (msg.vkcode == 'N' || msg.vkcode == VK_ESCAPE) {
				state = 0;
				greyified = 0;
				checked = 0;
			}
		}
		else if (state == 4) {
			if (msg.vkcode == 'Y' ||
				msg.vkcode == VK_RETURN || msg.vkcode == VK_SEPARATOR) {
				state = 0;
				nums.clear();
				for (int i = 0; i < 4; i++) {
					nums.push_back(std::pair<char, double>('a' + i, numbers[i]));
				}
			}
			else if (msg.vkcode == 'N' || msg.vkcode == VK_ESCAPE) {
				state = 0;
				greyified = 0;
			}
		}
		else if (msg.vkcode == VK_ESCAPE) {
			last_state = state;
			state = 5;
		}
	}
}
void Points24::exit() {
	Graph_IO::input.clear();
}

void Points24::generate_num() {
	if (!generated) {
		numbers.clear();
		nums.clear();
		for (int i = 0; i < 4; i++) {
			numbers.push_back(int_rand(1, 13));
			nums.push_back(std::pair<char, double>('a' + i, numbers[i]));
		}
		solvable = solve_24(numbers, solutions);
		generated = 1;
	}
	out = "使用" + std::to_string(numbers[0]) + ", " +
		std::to_string(numbers[1]) + ", " +
		std::to_string(numbers[2]) + ", " +
		std::to_string(numbers[3])+"计算24：";
	feedback.clear();
	fedback.clear();
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
			fedback += nums[i].first;
			std::string num = std::to_string(nums[i].second);
			fedback += " = " + num.
				substr(0, num.find('.') + 3);
			fedback += ", ";
		}
	}
	feedback += "uns->无解";
	fedback += "res->重来";
}
void Points24::compute(clock_t delta) {
	out = "输入结束。";
	if (Graph_IO::input.length() == 3) {
		if (Graph_IO::input == "uns") {
			state = 3;
			Graph_IO::input.clear();
			if (!greyified) {
				greyify_blur_screen();
				greyified = 1;
			}
		}
		else if (Graph_IO::input == "res") {
			state = 4;
			Graph_IO::input.clear();
			if (!greyified) {
				greyify_blur_screen();
				greyified = 1;
			}
		}
		else {
			char& x = Graph_IO::input[0];
			char& op = Graph_IO::input[1];
			char& y = Graph_IO::input[2];
			std::pair<char, double>* a = 0, * b = 0;
			for (std::pair<char, double>& num : nums) {
				if (x == num.first) {
					a = &num;
					break;
				}
			}
			for (std::pair<char, double>& num : nums) {
				if (y == num.first) {
					b = &num;
					break;
				}
			}
			mistaken = !a || !b;
			if (!mistaken) {
				if (x != y) {
					double result = 0;
					if (calculate(a->second, b->second, op, result)) {
						a->second = result;
						std::swap(nums.back(), *b);
						nums.pop_back();
						mistaken = 0;
						std::sort(nums.begin(), nums.end(),
							[](std::pair<char, double> a, std::pair<char, double> b) {
								return a.first < b.first;
							});
						Graph_IO::input.clear();
						if (nums.size() == 1) {
							if (abs(result - 24) < EPSILON) {
								generated = 0;
								state = 2;
							}
						}
						else {
							state = 0;
						}
					}
					else {
						mistaken = 1;
						if (proceed_loading(feedback, delta, 5, "输入运算符错误或除数为零。")) {
							state = 0;
							Graph_IO::input.clear();
						}
					}
				}
				else {
					mistaken = 1;
					if (proceed_loading(feedback, delta, 5, "输入左右字母相同。")) {
						state = 0;
						Graph_IO::input.clear();
					}
				}
			}
			else {
				if (proceed_loading(feedback, delta, 5, "输入有字母出界。")) {
					state = 0;
					Graph_IO::input.clear();
				}
			}
		}
	}
	else {
		mistaken = 1;
		if (proceed_loading(feedback, delta, 5, "输入长度必定为3。")) {
			state = 0;
			Graph_IO::input.clear();
		}
	}
}
void Points24::ask_regenerate() {
	out = "24点求解成功，是否再来【Y/N】?";
}
void Points24::confirm_solvable(clock_t delta) {
	if (!checked) {
		out = "是否确认无解【Y/N】?";
	}
	else {
		if (proceed_loading(out, delta, 5, "其实有解，更换题目中。")) {
			checked = 0;
			generated = 0;
			greyified = 0;
			state = 0;
		}
	}
}
void Points24::confirm_restart() {
		out = "是否确认重来【Y/N】?";
}
void Points24::ask_quit() {
	out = "是否返回主菜单【Y/N】?";
	if (!greyified) {
		greyify_blur_screen();
		greyified = 1;
	}
}
void Points24::quit(clock_t delta) {
	if (proceed_loading(out, delta, 5, "正在返回主菜单。")) {
		current_mode = "menu";
		ended = 1;
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