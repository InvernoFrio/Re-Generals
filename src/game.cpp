#include"game.h"
void Game::init() {
    //init window
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, PROJECT_NAME);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(DEFAULT_FPS);
    setPlayerNumber(DEFAULT_PLAYER_NUMBER);
    setBotNumber(DEFAULT_BOT_NUMBER);
    setSpeed(DEFAULT_SPEED);
    is_running = true;

    initControllerPool();
    setActivatedPlayer(PLAYER);

    initMap();
}
void Game::initControllerPool() {
    auto spectator = std::make_unique<Player>(SPECTATOR, WHITE);
    player_register[SPECTATOR] = spectator.get();
    controller_pool.push_back(std::move(spectator));
    for (int i = 1;i <= player_number;i++) {//player
        // auto player = std::make_unique<Player>(i, selectColor());
        auto player = std::make_unique<Player>(i, BLUE);
        player_register[i] = player.get();
        controller_pool.push_back(std::move(player));
    }
    for (int i = player_number + 1;i <= player_number + bot_number;i++) {//bot
        controller_pool.push_back(std::make_unique<Bot>(i, selectColor()));
    }
}
void Game::initMap() {
    map.init();
    refreshMap();
}
void Game::setActivatedPlayer(int id) {
    activated_player_ptr = player_register[id];
}
void Game::update() {
    if (frame_number % speed == 0) {
        round = frame_number / speed;
        updateEconomy();
    }
    updateControllers();
}
void Game::render() {
    frame_number++;
    BeginDrawing();

    //绘制游戏内容
    BeginMode2D(activated_player_ptr->getCamera());
    ClearBackground(BACKGUROND_COLOR);
    map.draw(activated_player_ptr);
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
void Game::updateControllers() {
    for (auto& controller : controller_pool) {
        controller->update();
    }
}
void Game::setPlayerNumber(int player_number) {
    this->player_number = player_number;
}
void Game::setBotNumber(int bot_number) {
    this->bot_number = bot_number;
}
void Game::setSpeed(int speed) {
    this->speed = speed;
}
Color Game::selectColor() {
    srand(time(0));
    int p = rand() % MAX_PLAYER_NUMBER;
    while (color_selected[p])p = (p + 1) % MAX_PLAYER_NUMBER;
    color_selected[p] = 1;
    return color_pool[p];
}
void Game::updateEconomy() {
    for (int i = 1;i <= map.getHeight();i++) {
        for (int j = 1;j <= map.getWidth();j++) {
            Square& now = map.map[i][j];
            if (now.getId() == 0)continue;
            if (now.getType() == TYPE_LAND && round % 25 == 0 && round != 0)now.grow();
            if ((now.getType() == TYPE_CITY || now.getType() == TYPE_GENERAL) && round % 2 == 0 && round != 0)now.grow();
        }
    }
}
void Game::generateObstacles() {
    do {
        generateMountains();
        generateCities();
    } while (!checkMapConnectivity());
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
        now.setColor(controller_pool[i]->getColor());
        now.setId(i);
        now.setSolderNum(1);
    }
}
bool Game::checkMapConnectivity() {
    std::queue<std::pair<int, int> >q;
    for (int i = 1;i <= map.getWidth();i++) {
        q.push(pii(1, i));
    }
    bool vis[map.getHeight() + 1][map.getWidth() + 1];
    memset(vis, 0, sizeof(vis));
    pii now, to;
    pii dr[2] = { pii(1,0),pii(0,1) };
    while (q.size()) {
        now = q.front();q.pop();
        if (vis[now.first][now.second] == 1)continue;
        vis[now.first][now.second] = 1;
        for (int k = 0;k < 2;k++) {
            to.first += now.first, to.second += now.second;
            if (to.first > map.getHeight() || to.second > map.getWidth())continue;
            int type = map.map[to.first][to.second].getType();
            if (type == TYPE_LAND || type == TYPE_GENERAL) {
                q.push(to);
            }
        }
    }
    bool flag = 1;
    for (int i = 1;i <= map.getHeight();i++) {
        for (int j = 1;j <= map.getWidth();j++) {
            if (vis[i][j] == 0) {
                flag = 0;
                break;
            }
        }
        if (!flag)break;
    }
    return 1;
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
            now.setType(TYPE_LAND);
            now.setSolderNum(0);
            now.setColor(DEFAULT_COLOR_LAND);
            now.setId(0);
        }
    }
}