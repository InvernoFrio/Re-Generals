#include"player.h"

Player::Player(int id, Color color, Square(*map)[MAX_MAP_WIDTH + 1]) {
    this->setId(id);
    this->setColor(color);
    is_alive = true;
    is_dragging = false;
    last_select_pos = Pos(0, 0, NO_ARROW);
    land_num = 1;
    solder_num = 1;
    this->map = map;
    timer = clock();
    initCamera();
}
void Player::initCamera() {
    camera = { 0 };
    last_screen_width = GetScreenWidth();
    last_screen_height = GetScreenHeight();
    camera.target = (Vector2){ 0,0 };
    camera.offset = (Vector2){ 0,0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    is_dragging = false;
    updateCameraOffset();
}
void Player::update() {
    updateCamera();
    if (is_alive && id != SPECTATOR) {
        handleMouseInput();
        handleKBInput();
    }
}
void Player::handleKBInput() {
    Pos dr[4] = { Pos(0,-1,NO_ARROW),Pos(0,1,NO_ARROW),Pos(-1,0,NO_ARROW),Pos(1,0,NO_ARROW) };
    int direction = -1;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))direction = 0;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))direction = 1;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))direction = 2;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))direction = 3;
    if (direction != -1 && checkMotionQueueLimit() && clock() - timer >= MOTION_INVERVAL) {
        timer = clock();
        int tox = last_select_pos.x + dr[direction].x;
        int toy = last_select_pos.y + dr[direction].y;
        if (tox >= 1 && tox <= map_height && toy >= 1 && toy <= map_width) {
            updateSelectedSquare(tox, toy, true);
        }
    }
    if (IsKeyPressed(KEY_Q))clearMotionQueue();
    if (IsKeyDown(KEY_E) && clock() - timer >= UNDO_INVERVAL) {
        timer = clock();
        undoMotionQueue();
    }
}
void Player::clearMotionQueue() {
    while (motion_queue.size()) {
        Pos now = motion_queue.front();
        map[now.x][now.y].setArrow(id, now.dir, false);
        motion_queue.pop_front();
    }
}
void Player::undoMotionQueue() {
    if (motion_queue.size()) {
        Pos now = motion_queue.back();
        updateSelectedSquare(now.x, now.y, false);
        map[now.x][now.y].setArrow(id, now.dir, false);
        motion_queue.pop_back();
    }
}
void Player::handleMouseInput() {
    Vector2 mouse_world_pos = GetScreenToWorld2D(GetMousePosition(), camera);
    int x = mouse_world_pos.x / SQUARE_SIZE + 1;
    int y = mouse_world_pos.y / SQUARE_SIZE + 1;


    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (checkMotionQueueLimit())updateSelectedSquare(x, y, true);
    }
}
void Player::updateSelectedSquare(int x, int y, bool flag) {
    int dx[4] = { 0,0,-1,1 };
    int dy[4] = { -1,1,0,0 };
    if (x >= 1 && x <= map_height && y >= 1 && y <= map_width) {//select inside map
        int arrow = map[x][y].aroundedBy(id);
        if (arrow != NO_ARROW && flag) {
            motion_queue.push_back(Pos(last_select_pos.x, last_select_pos.y, arrow));
            map[last_select_pos.x][last_select_pos.y].setArrow(id, arrow, true);
        }
        if (last_select_pos.x != 0 && last_select_pos.y != 0) {//clear last select state
            map[last_select_pos.x][last_select_pos.y].setSelectState(id, false);
            for (int i = 0;i < 4;i++) {
                int tox = last_select_pos.x + dx[i];
                int toy = last_select_pos.y + dy[i];
                if (tox<1 || tox>map_height || toy<1 || toy>map_width)continue;
                map[tox][toy].setAroundedState(id, NO_ARROW);
            }
        }
        map[x][y].setSelectState(id, true);
        std::cout << "Player select square (" << x << ' ' << y << ")" << std::endl;
        for (int i = 0;i < 4;i++) {
            int tox = x + dx[i], toy = y + dy[i];
            if (tox<1 || tox>map_height || toy<1 || toy>map_width) continue;
            map[tox][toy].setAroundedState(id, i + ARROWUP);
        }
        last_select_pos = Pos(x, y, NO_ARROW);
    }
    else {//select outside the map
        if (last_select_pos.x != 0 && last_select_pos.y != 0) {//clear last select state
            map[last_select_pos.x][last_select_pos.y].setSelectState(id, false);
            for (int i = 0;i < 4;i++) {
                int tox = last_select_pos.x + dx[i];
                int toy = last_select_pos.y + dy[i];
                if (tox<1 || tox>map_width || toy<1 || toy>map_height)continue;
                map[tox][toy].setAroundedState(id, NO_ARROW);
            }
        }
        last_select_pos = Pos(0, 0, NO_ARROW);
    }
}
void Player::updateCamera() {
    if (last_screen_height != GetScreenHeight() || last_screen_width != GetScreenWidth()) {
        updateCameraOffset();
    }
    handleCameraInput();
}
void Player::handleCameraInput() {
    float wheel = GetMouseWheelMove();

    if (wheel != 0) {
        Vector2 mouseWorldPosBefore = GetScreenToWorld2D(GetMousePosition(), camera);
        camera.zoom += wheel * 0.1f;
        camera.zoom = Clamp(camera.zoom, 0.3f, 3.0f);
        Vector2 mouseWorldPosAfter = GetScreenToWorld2D(GetMousePosition(), camera);

        camera.target.x -= (mouseWorldPosAfter.x - mouseWorldPosBefore.x);
        camera.target.y -= (mouseWorldPosAfter.y - mouseWorldPosBefore.y);
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        is_dragging = true;
        camera_last_pos = GetScreenToWorld2D(GetMousePosition(), camera);
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && is_dragging) {
        Vector2 camera_current_pos = GetScreenToWorld2D(GetMousePosition(), camera);
        camera.target.x -= (camera_current_pos.x - camera_last_pos.x);
        camera.target.y -= (camera_current_pos.y - camera_last_pos.y);
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        is_dragging = false;
    }

    if (IsKeyPressed(KEY_R)) {
        resetCamera();
    }
}
void Player::updateCameraOffset() {
    camera.offset = (Vector2){
        GetScreenWidth() / 2.0f,
        GetScreenHeight() / 2.0f
    };
    last_screen_width = GetScreenWidth();
    last_screen_height = GetScreenHeight();
}
Camera2D Player::getCamera() {
    return this->camera;
}
bool Player::checkMotionQueueLimit() {
    return motion_queue.size() <= MAX_MONTION_NUMBER;
}
void Player::resetCamera() {
    camera.target = { 1.0f * map_height / 2 * SQUARE_SIZE,1.0f * map_width / 2 * SQUARE_SIZE };
    camera.zoom = 0.8f;
    updateCameraOffset();
}
Pos Player::getMotion() {
    if (motion_queue.size()) {
        Pos now = motion_queue.front();
        map[now.x][now.y].setArrow(id, now.dir, false);
        motion_queue.pop_front();
        return now;
    }
    return Pos(0, 0, NO_ARROW);
}