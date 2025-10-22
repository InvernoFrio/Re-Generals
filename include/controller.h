#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include"raylib/raylib.h"
#include"raylib/raymath.h"
class Controller {
private:
    int id;
    int land_num;
    bool is_alive, is_dragging;
    int last_screen_width, last_screen_height;
    Vector2 camera_last_pos;
    Color color;
    Camera2D camera;
public:
    Controller();
    Controller(int id, Color color);
    int getId();
    Color getColor();
    void setColor(Color color);
    void handlerCameraInput();
    void init();
    void initCamera();
    void update();
    void updateCamera();
    void updateCameraOffset();
    Camera2D getCamera();
};

#endif