#include"raylib/raylib.h"
#include"raylib/raymath.h"
#include"game.h"
void Game::init() {
    //init window
    InitWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, PROJECT_NAME);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(DEFAULT_FPS);

    //init texture_pool
    textures_pool.init();

    is_running = true;
    //init controller
    controller_pool.push_back(Controller());
    setPlayerNumber(DEFAULT_PLAYER_NUMBER);
    setMainController(controller_pool[0]);

}
void Game::update() {
    main_contoller.update();
}
void Game::render() {
    BeginDrawing();

    //绘制游戏内容
    BeginMode2D(main_contoller.getCamera());
    ClearBackground(BACKGUROND_COLOR);
    DrawTexture(textures_pool.get(GENERAL), 0, 0, WHITE);
    map.drawMap(main_contoller);
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
    textures_pool.unloadAll();
    CloseWindow();
}
void Game::setMainController(Controller& controller) {
    this->main_contoller = controller;
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