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
    auto spectator = std::make_unique<Player>(SPECTATOR, WHITE, this->map.map);
    player_register[SPECTATOR] = spectator.get();
    controller_pool.push_back(std::move(spectator));
    for (int i = 1;i <= player_number;i++) {//player
        auto player = std::make_unique<Player>(i, selectColor(), this->map.map);
        player_register[i] = player.get();
        controller_pool.push_back(std::move(player));
    }
    for (int i = player_number + 1;i <= player_number + bot_number;i++) {//bot
        controller_pool.push_back(std::make_unique<Bot>(i, selectColor(), this->map.map));
    }
}
void Game::initMap() {
    map.init();
    setHeight(DEFAULT_MAP_HEIGHT);
    setWidth(DEFAULT_MAP_WIDTH);
    setCityNumber(DEFAULT_CITY_NUMBER);
    setMountainNumber(DEFAULT_MOUNTAIN_NUMBER);
    refreshMap();
    std::cout << "Init successfully" << std::endl;
}
void Game::setActivatedPlayer(int id) {
    activated_player_ptr = player_register[id];
    activated_player_ptr->setMapHeight(map_height);
    activated_player_ptr->setMapWidth(map_width);
    activated_player_ptr->resetCamera();
}
void Game::update() {
    updateControllers();
    if (frame_number % speed == 0) {
        std::cout << "round " << round << std::endl;
        round = frame_number / speed;
        updateEconomy();
    }
}
void Game::render() {
    frame_number++;
    BeginDrawing();

    //绘制游戏内容
    BeginMode2D(activated_player_ptr->getCamera());
    ClearBackground(BACKGUROND_COLOR);
    map.draw(activated_player_ptr, map_height, map_width);
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
    map.texture_pool.unloadAll();
    CloseWindow();
}
void Game::updateControllers() {
    for (auto& controller : controller_pool) {
        if (controller->isAlive())controller->update();
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
    for (int i = 1;i <= map_height;i++) {
        for (int j = 1;j <= map_width;j++) {
            Square& now = map.map[i][j];
            if (now.getId() == 0)continue;
            if (now.getType() == TYPE_LAND && round % 50 == 0 && round != 0)now.grow();
            if ((now.getType() == TYPE_CITY || now.getType() == TYPE_GENERAL) && round % 2 == 0 && round != 0)now.grow();
        }
    }
    for (auto& controller : controller_pool) {
        if (controller->isAlive() == false)continue;
        int dx[4] = { 0,0,-1,1 };
        int dy[4] = { -1,1,0,0 };
        Pos motion = controller->getMotion();
        if (motion.dir == NO_ARROW)continue;
        int x = motion.x, y = motion.y;
        int tox = x + dx[motion.dir - ARROWUP], toy = y + dy[motion.dir - ARROWUP];
        int id_now = controller->getId();
        Square& now = map.map[x][y];
        Square& to = map.map[tox][toy];
        if (controller->getId() != now.getId()) {
            controller->clearMotionQueue();
            continue;
        }
        if (to.getType() == TYPE_MOUNTAIN) {
            controller->clearMotionQueue();
            continue;
        }
        if (to.getId() == now.getId()) {
            to.setSolderNum(to.getSolderNum() + now.getSolderNum() - 1);
            now.setSolderNum(1);
            continue;
        }
        auto& target_ptr = controller_pool[to.getId()];
        if (to.getSolderNum() - (now.getSolderNum() - 1) >= 0) {
            to.setSolderNum(to.getSolderNum() - now.getSolderNum() + 1);
            now.setSolderNum(1);
            target_ptr->setSolderNum(target_ptr->getSolderNum() - now.getSolderNum() + 1);
            controller->setSolderNum(target_ptr->getSolderNum() - now.getSolderNum() + 1);
        }
        else {
            to.setColor(now.getColor());
            to.setSolderNum(now.getSolderNum() - 1 - to.getSolderNum());
            to.setId(now.getId());
            now.setSolderNum(1);

            target_ptr->setLandNum(controller->getLandNum() - 1);
            controller->setLandNum(controller->getLandNum() + 1);
            if (to.getType() == TYPE_GENERAL) {
                killController(to.getId(), now.getId());
                std::cout << "player " << controller->getId() << " captured player " << target_ptr->getId() << std::endl;
            }
        }
    }
}
void Game::killController(int target, int by) {
    auto& death_ptr = controller_pool[target];
    auto& killer_ptr = controller_pool[by];
    death_ptr->kill();

    for (int i = 1;i <= map_height;i++) {
        for (int j = 1;j <= map_width;j++) {
            Square& now = map.map[i][j];
            if (now.getId() == target) {
                now.setColor(killer_ptr->getColor());
                now.setId(by);
            }
            if (now.getType() == TYPE_GENERAL) {
                now.setType(TYPE_CITY);
            }
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
    for (int i = 1;i <= city_number;i++) {
        int x, y;
        do {
            x = (rand() % map_height) + 1;
            y = (rand() % map_width) + 1;
        } while (map.map[x][y].getType() != TYPE_LAND);
        Square& now = map.map[x][y];
        now.setType(TYPE_MOUNTAIN);
        now.setColor(DEFAULT_COLOR_MOUNTAIN);
    }
}
void Game::generateCities() {
    srand(time(0));
    for (int i = 1;i <= mountain_number;i++) {
        int x, y;
        do {
            x = (rand() % map_height) + 1;
            y = (rand() % map_width) + 1;
        } while (map.map[x][y].getType() != TYPE_LAND);
        Square& now = map.map[x][y];
        now.setType(TYPE_CITY);
        now.setColor(DEFAULT_COLOR_CITY);
        now.setSolderNum(rand() % 20 + 40);
    }
}
void Game::generateGeneral() {
    for (int i = 1;i <= player_number + bot_number;i++) {
        srand(time(0));
        int x, y;
        do {
            x = (rand() % map_height) + 1;
            y = (rand() % map_width) + 1;
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
    for (int i = 1;i <= map_width;i++) {
        q.push(pii(1, i));
    }
    bool vis[map_height + 1][map_width + 1];
    memset(vis, 0, sizeof(vis));
    pii now, to;
    pii dr[2] = { pii(1,0),pii(0,1) };
    while (q.size()) {
        now = q.front();q.pop();
        if (vis[now.first][now.second] == 1)continue;
        vis[now.first][now.second] = 1;
        for (int k = 0;k < 2;k++) {
            to.first += now.first, to.second += now.second;
            if (to.first > map_height || to.second > map_width)continue;
            int type = map.map[to.first][to.second].getType();
            if (type == TYPE_LAND || type == TYPE_GENERAL) {
                q.push(to);
            }
        }
    }
    bool flag = 1;
    for (int i = 1;i <= map_height;i++) {
        for (int j = 1;j <= map_width;j++) {
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
    for (int i = 1;i <= map_height;i++) {
        for (int j = 1;j <= map_width;j++) {
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
bool Game::setHeight(int map_height) {
    if (map_height <= MAX_MAP_HEIGHT) {
        this->map_height = map_height;
        return 1;
    }
    else {
        throw std::runtime_error("reached map heigt limit");
        return 0;
    }
}
bool Game::setWidth(int map_width) {
    if (map_width <= MAX_MAP_WIDTH) {
        this->map_width = map_width;
        return 1;
    }
    else {
        throw std::runtime_error("reached map width limit");
        return 0;
    }
}
bool Game::setCityNumber(int city_number) {
    this->city_number = city_number;
    return 1;
}
bool Game::setMountainNumber(int mountain_number) {
    this->mountain_number = mountain_number;
    return 1;
}