#pragma once
#include "main_menu.h"
#include "arith_login.h"
#include "arith_admin.h"
#include "guess_number.h"
#include "points_24.h"

class SceneController {
private:
	Scene* current = nullptr;
public:
	SceneController() = default;
	~SceneController() = default;

	void switch_list(SceneType type);
	void set(SceneType type);
	void switch_to(SceneType type);

	void draw();
	void timekeep(clock_t delta);
	void proceed();
	void input(const ExMessage& msg);
};