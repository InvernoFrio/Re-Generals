#include"controller.h"

Controller::Controller(int id, Color color) :id(id), color(color) {
    init();
}
Controller::Controller() {
    init();
}

Color Controller::getColor() { return this->color; }
int Controller::getId() { return this->id; }
void Controller::setColor(Color color) { this->color = color; }
void Controller::init() {
    is_alive = true;
    is_dragging = false;
    land_num = 1;
    initCamera();
}
void Controller::initCamera() {
    camera = { 0 };
    camera.target = (Vector2){ 0,0 };
    camera.offset = (Vector2){ 0,0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    is_dragging = false;
    last_screen_width = GetScreenWidth();
    last_screen_height = GetScreenHeight();
    updateCameraOffset();
}
void Controller::update() {
    updateCamera();
}
void Controller::updateCamera() {
    if (last_screen_height != GetScreenHeight() || last_screen_width != GetScreenWidth()) {
        updateCameraOffset();
    }
    handlerCameraInput();
}
void Controller::handlerCameraInput() {
    float wheel = GetMouseWheelMove();

    if (wheel != 0) {
        Vector2 mouseWorldPosBefore = GetScreenToWorld2D(GetMousePosition(), camera);
        camera.zoom += wheel * 0.1f;
        camera.zoom = Clamp(camera.zoom, 0.5f, 5.0f);
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
        camera.target = (Vector2){ 0,0 };
        camera.zoom = 1.0f;
        updateCameraOffset();
    }
}
void Controller::updateCameraOffset() {
    camera.offset = (Vector2){
        GetScreenWidth() / 2.0f,
        GetScreenHeight() / 2.0f
    };
    last_screen_width = GetScreenWidth();
    last_screen_height = GetScreenHeight();
}
Camera2D Controller::getCamera() {
    return this->camera;
}