#ifndef RENDER_H
#define RENDER_H
#include"raylib/raylib.h"
#include"raylib/raymath.h"
#include"constants.h"
#include"map.h"
#include<mutex>
#include<atomic>


class Render {
private:
    int id;
public:
    std::atomic<bool>current_write{ 0 };
    std::atomic<bool>current_read{ 1 };
    Camera2D camera;
    int last_screen_height, last_screen_width;
    bool is_dragging;
    Vector2 camera_last_pos;
    Map* map_ptr;
    std::mutex mtx;

public:
    void init(int id, Map* map_ptr);
    void render();
    void drawUI();
    void drawMap();
    void initCamera();

    char getInput();

    void handleCameraInput();
    void resetCamera();

    void updateCamera();

    void updateCameraOffset();
    ~Render();
};

#endif // RENDER_H