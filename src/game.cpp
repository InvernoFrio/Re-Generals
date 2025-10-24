#include"raylib/raylib.h"
#include"raylib/raymath.h"
#include"game.h"
void Game::init() {
    //init window
    InitWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, PROJECT_NAME);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(DEFAULT_FPS);

    //init map
    map.init();

    is_running = true;
    //init controller
    controller_pool.push_back(Controller(SPECTATOR, WHITE));
    setPlayerNumber(DEFAULT_PLAYER_NUMBER);
    setSpectator(controller_pool[SPECTATOR]);

}
void Game::update() {
    spectator.update();
}
void Game::render() {
    BeginDrawing();

    //绘制游戏内容
    BeginMode2D(spectator.getCamera());
    ClearBackground(BACKGUROND_COLOR);
    map.drawMap(spectator);
    EndMode2D();

    //绘制UI内容
    //TODO

    EndDrawing();
}
void Game::run() {
    init();
    while (!WindowShouldClose()) {
        update();
        render();
    }
    cleanup();
}
void Game::cleanup() {
    map.textures_pool.unloadAll();
    CloseWindow();
}
void Game::setSpectator(Controller& controller) {
    this->spectator = controller;
}
void Game::setPlayerNumber(int player_number) {
    this->player_number = player_number;
    if (controller_pool.size() - 1 > player_number) {
        for (int i = 1;i <= controller_pool.size() - player_number;i++)controller_pool.pop_back();
    }
    else if (controller_pool.size() - 1 < player_number) {
        for (int i = controller_pool.size();i <= player_number;i++)controller_pool.push_back(Controller(i, selectColor()));
    }
}
Color Game::selectColor() {
    return WHITE;
}