#include"raylib/raylib.h"
#include"raylib/raymath.h"
#include"game.h"
void Game::init() {
    //init window
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, PROJECT_NAME);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(DEFAULT_FPS);
    is_running = true;

    //init map
    initMap();

    //init controller
    controller_pool.push_back(Controller(SPECTATOR, WHITE));
    setPlayerNumber(DEFAULT_PLAYER_NUMBER);
    setSpectator(controller_pool[SPECTATOR]);
}
void Game::initMap() {
    map.init();
    map.setHeight(DEFAULT_MAP_HEIGHT);
    map.setWidth(DEFAULT_MAP_WIDTH);
    map.setCityNumber(DEFAULT_CITY_NUMBER);
    map.setMountainNumber(DEFAULT_MOUNTAIN_NUMBER);
    refreshMap();
}
void Game::update() {
    spectator.update();
}
void Game::render() {
    BeginDrawing();

    //绘制游戏内容
    BeginMode2D(spectator.getCamera());
    ClearBackground(BACKGUROND_COLOR);
    map.draw(spectator);
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
    if (player_number >= MAX_PLAYER_NUMBER || player_number <= 1)return;
    this->player_number = player_number;
    if (controller_pool.size() - 1 > player_number) {
        for (int i = 1;i <= controller_pool.size() - player_number;i++)controller_pool.pop_back();
    }
    else if (controller_pool.size() - 1 < player_number) {
        for (int i = controller_pool.size();i <= player_number;i++)controller_pool.push_back(Controller(i, selectColor(i)));
    }
}
Color Game::selectColor(int id) {
    srand(time(0));
    int p = rand() % MAX_PLAYER_NUMBER;
    while (color_selected[p])p = (p + 1) % MAX_PLAYER_NUMBER;
    color_selected[p] = 1;
    return color_pool[p];
}
void Game::updateEconomy() {

}
void Game::generateObstacles() {
    do {
        generateMountains();
        generateCities();
    } while (!checkConnectivity());
}
void Game::generateMountains() {
    srand(time(0));
    for (int i = 1;i <= map.getCityNumber();i++) {
        int x, y;
        do {
            x = (rand() % map.getHeight()) + 1;
            y = (rand() % map.getWidth()) + 1;
        } while (map.map[x][y].getType() != TYPE_LAND);
        Square& now = map.map[x][y];
        now.setType(TYPE_MOUNTAIN);
        now.setColor(DEFAULT_COLOR_MOUNTAIN);
    }
}
void Game::generateCities() {
    srand(time(0));
    for (int i = 1;i <= map.getMountainNumber();i++) {
        int x, y;
        do {
            x = (rand() % map.getHeight()) + 1;
            y = (rand() % map.getWidth()) + 1;
        } while (map.map[x][y].getType() != TYPE_LAND);
        Square& now = map.map[x][y];
        now.setType(TYPE_CITY);
        now.setColor(DEFAULT_COLOR_CITY);
        now.setSolderNum(rand() % 20 + 40);
        now.setSpawnSpeed(INF);
    }
}
void Game::generateGeneral() {
    for (int i = 1;i <= player_number;i++) {
        srand(time(0));
        int x, y;
        do {
            x = (rand() % map.getHeight()) + 1;
            y = (rand() % map.getWidth()) + 1;
        } while (map.map[x][y].getType() != TYPE_LAND);
        Square& now = map.map[x][y];
        now.setType(TYPE_GENERAL);
        now.setColor(controller_pool[i].getColor());
        now.setId(i);
        now.setSolderNum(1);
        now.setSpawnSpeed(2);
    }
}
bool Game::checkConnectivity() {//TODO
    return true;
}
void Game::refreshMap() {
    clearMap();
    generateObstacles();
    generateGeneral();
}

void Game::clearMap() {
    for (int i = 1;i <= map.getHeight();i++) {
        for (int j = 1;j <= map.getWidth();j++) {
            Square& now = map.map[i][j];
            now.setX((i - 1) * SQUARE_SIZE);
            now.setY((j - 1) * SQUARE_SIZE);
            now.setSpawnSpeed(INF);
            now.setType(TYPE_LAND);
            now.setSolderNum(0);
            now.setColor(DEFAULT_COLOR_LAND);
            now.setId(0);
        }
    }
}