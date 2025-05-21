#include "scene_controller.h"

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
	Graphio::solid_rectangle(LIGHTGRAY, 0, 0, WIDTH, HEIGHT);
	setlinestyle(PS_SOLID, 2);
	Graphio::fill_round_rectangle(WHITE, BLACK,
		SIDE / 5, SIDE / 5,
		WIDTH - SIDE / 5, HEIGHT - SIDE / 5,
		SIDE * 2 / 5, SIDE * 2 / 5);
	current->draw();
}
void SceneController::input(const ExMessage& msg) {
	current->input(msg);
}