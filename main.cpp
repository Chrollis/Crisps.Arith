#include "controller.h"

int WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd) {
    SceneController game;
    FpsController fps_controller;
    ExMessage msg;
    clock_t last_tick = clock();
    clock_t current_tick = clock();
    HANDLE m_hFont = 0;

    set_font(m_hFont);
    initgraph(WIDTH, HEIGHT);
    fps_controller.set(FPS);

    BeginBatchDraw();

    game.set(STMainMenu);

    while (1) {
        fps_controller.start();

        while (peekmessage(&msg)) {
            game.input(msg);
        }

        current_tick = clock();
        game.proceed(current_tick - last_tick);
        last_tick = current_tick;

        cleardevice();
        game.draw();
        //fps_controller.draw();

        if (Scene::ended) {
            if (Scene::current_mode == "kousuan") {
                game.switch_to(STArithLogin);
            }
            else if (Scene::current_mode == "menu") {
                game.switch_to(STMainMenu);
            }
            else if (Scene::current_mode == "exit") {
                break;
            }
            else if (Scene::current_mode == "arithmetic") {
                game.switch_to(STArithmetic);
            }
            else if (Scene::current_mode == "arith_admin") {
                game.switch_to(STArithAdmin);
            }
            else if (Scene::current_mode == "caishuzi") {
                game.switch_to(STGuessNumber);
            }
            else if (Scene::current_mode == "24dian") {
                game.switch_to(STPoints24);
            }
        }

        FlushBatchDraw();

        if (Scene::ended) {
            Scene::ended = 0;
        }
        else {
            fps_controller.delay();
        }
    }
    EndBatchDraw();

    closegraph();
    if (!m_hFont) {
        return 0;
    }
    RemoveFontMemResourceEx(m_hFont);

    return 0;
}