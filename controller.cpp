#include "controller.h"

void FpsController::start() {
	begin = clock();
}
void FpsController::delay()const {
	Sleep((DWORD)(max(0, interval + clock() - begin)));
}
void FpsController::set(const int fps) {
	interval = 500 / fps;
	fps_target = fps;
}
void FpsController::draw() {
	static int counter = 0;
	if (counter == 0) {
		actual_fps = 1000 * fps_target / (clock() - end);
		end = clock();
	}
	counter = (++counter) % fps_target;
	std::ostringstream oss;
	oss << "Fps = " << actual_fps << " : " << fps_target;
	Graph_IO::set_text_style(L"Ï¼ðÍÂþºÚ", SIDE);
	setbkmode(TRANSPARENT);
	Graph_IO::output_text(GOLDENROD1, SIDE / 2, HEIGHT - SIDE * 6 / 5, oss.str());
}

void SceneController::switch_list(SceneType type) {
	switch (type) {
	case STMainMenu:
		current = new MainMenu;
		break;
	case STArithLogin:
		current = new ArithLogin;
		break;
	case STArithmetic:
		current = new Arithmetic;
		break;
	case STArithAdmin:
		current = new ArithAdmin;
		break;
	case STGuessNumber:
		current = new GuessNumber;
		break;
	case STPoints24:
		current = new Points24;
		break;
	default:
		break;
	}
}
void SceneController::set(SceneType type) {
	switch_list(type);
	current->enter();
}
void SceneController::switch_to(SceneType type) {
	current->exit();
	delete current;
	switch_list(type);
	current->enter();
}
void SceneController::timekeep(clock_t delta) {
	current->timekeep(delta);
}
void SceneController::proceed() {
	current->proceed();
}
void SceneController::draw() {
	Graph_IO::solid_rectangle(LIGHTGRAY, 0, 0, WIDTH, HEIGHT);
	setlinestyle(PS_SOLID, 2);
	Graph_IO::fill_round_rectangle(WHITE, BLACK,
		SIDE / 5, SIDE / 5,
		WIDTH - SIDE / 5, HEIGHT - SIDE / 5,
		SIDE * 2 / 5, SIDE * 2 / 5);
	current->draw();
}
void SceneController::input(const ExMessage& msg) {
	current->input(msg);
}