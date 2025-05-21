#include "scene.h"

IMAGE Scene::screen = IMAGE(WIDTH, HEIGHT);
RECT Scene::screen_rect = { 0,0,WIDTH,HEIGHT };
RECT Scene::question_rect = { SIDE / 2,HEIGHT / 2 - SIDE * 9 / 5,
	WIDTH - SIDE / 2,HEIGHT / 2 - SIDE * 4 / 5 };
RECT Scene::input_rect = { SIDE * 3 / 5,HEIGHT / 2 - SIDE * 3 / 5,
	WIDTH - SIDE * 3 / 5,HEIGHT / 2 + SIDE * 2 / 5 };
RECT Scene::feedback_rect = { SIDE / 2,HEIGHT / 2 + SIDE * 3 / 5,
WIDTH - SIDE / 2,HEIGHT / 2 + SIDE * 8 / 5 };
std::vector<int> Scene::input_line_position = { SIDE / 2,HEIGHT / 2 + SIDE * 2 / 5,
WIDTH - SIDE / 2,HEIGHT / 2 + SIDE * 2 / 5 };

std::string Scene::current_id;
std::string Scene::current_mode;
bool Scene::current_ended = 0;

void Scene::enter_loading(const int circle, const std::string& out_str) {
	Graphio::set_text_style(L"Ï¼ðÍÂþºÚ");
	setbkmode(TRANSPARENT);
	setlinestyle(PS_SOLID, 2);
	const std::string loading = "/-\\-";
	std::string temp;

	for (int i = 0; i < circle; i++) {
		Graphio::solid_rectangle(LIGHTGRAY, 0, 0, WIDTH, HEIGHT);
		setlinestyle(PS_SOLID, 2);
		Graphio::fill_round_rectangle(WHITE, BLACK,
			(int)(0.2 * SIDE), (int)(0.2 * SIDE),
			WIDTH - (int)(0.2 * SIDE), HEIGHT - (int)(0.2 * SIDE),
			(int)(0.4 * SIDE), (int)(0.4 * SIDE));
		temp = out_str + loading[i % 4];
		Graphio::draw_text(BLACK, temp, &screen_rect,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		FlushBatchDraw();
		Sleep(WAIT);
	}
}
void Scene::greyify_blur_screen(double para, int radius, double sigma) {
	getimage(&screen, 0, 0, WIDTH, HEIGHT);
	DWORD* p_screen = GetImageBuffer(&screen);
	for (int i = 0; i < WIDTH * HEIGHT; i++) {
		p_screen[i] = RGBtoGRAY(int(0.6 * p_screen[i]));
	}
	IMAGE temp_screen = IMAGE(WIDTH, HEIGHT);
	Graphio::gaussian_blur(&screen, &temp_screen, radius, sigma);
	DWORD* p_temp_screen = GetImageBuffer(&temp_screen);
	for (int i = 0; i < WIDTH * HEIGHT; i++) {
		p_screen[i] = RGBtoGRAY(p_temp_screen[i]);
	}
}

void Scene::normal_draw() const {
	Graphio::output_line(DARKGRAY, input_line_position);
	Graphio::draw_text(BLACK, out, &question_rect,
		DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	Graphio::draw_text(MIDNIGHTBLUE, Graphio::input + "<-", &input_rect,
		DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
}
void Scene::pop_up_draw() const {
	putimage(0, 0, &screen);
	Graphio::fill_round_rectangle(WHITE, BLACK,
		SIDE / 2, HEIGHT / 2 - SIDE * 3 / 5,
		WIDTH - SIDE / 2, HEIGHT / 2 + SIDE * 3 / 5,
		SIDE * 2 / 5, SIDE * 2 / 5);
	Graphio::draw_text(BLACK, out, &screen_rect,
		DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

