#include "render.h"

void Render::init(int id, Map* map_ptr) {
    this->map_ptr = map_ptr;
    this->id = id;
    InitWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, PROJECT_NAME);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
}
void Render::render() {
    updateCamera();
    std::lock_guard<std::mutex> lock(mtx);
    BeginDrawing();
    ClearBackground(BACKGUROND_COLOR);
    BeginMode2D(camera);
    drawMap();
    EndMode2D();
    drawUI();
    EndDrawing();
}
char Render::getInput() {
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))return 'w';
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))return 's';
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))return 'a';
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))return 'd';
    if (IsKeyPressed(KEY_Q))return 'q';
    if (IsKeyDown(KEY_E))return 'e';
    return '\0';
}
void Render::updateCamera() {
    if (last_screen_height != GetScreenHeight() || last_screen_width != GetScreenWidth()) {
        updateCameraOffset();
    }
    handleCameraInput();
}
void Render::initCamera() {
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
void Render::updateCameraOffset() {
    camera.offset = (Vector2){
        GetScreenWidth() / 2.0f,
        GetScreenHeight() / 2.0f
    };
    last_screen_width = GetScreenWidth();
    last_screen_height = GetScreenHeight();
}
void Render::handleCameraInput() {
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
void Render::drawMap() {
    std::lock_guard<std::mutex> lock(mtx);
    Map& data = *map_ptr;
    int height = data.getHeight();
    int width = data.getWidth();
    for (int i = 0;i < height;i++) {
        for (int j = 0;j < width;j++) {
            Square& now = data.getSquare(i, j);
            // Render each square based on its properties
            // e.g., draw rectangles, textures, etc.
        }
    }
    // Implement rendering logic here using the data
}
void Render::drawUI() {

}
void Render::resetCamera() {
    camera.target = (Vector2){ 0,0 };
    camera.zoom = 1.0f;
    updateCameraOffset();
}
Render::~Render() {
    ::CloseWindow();
}