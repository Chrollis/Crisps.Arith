#include "fps_controller.h"

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
	Graphio::set_text_style(L"Ï¼ðÍÂþºÚ", SIDE);
	setbkmode(TRANSPARENT);
	Graphio::output_text(GOLDENROD1, SIDE / 2, HEIGHT - SIDE * 6 / 5, oss.str());
}