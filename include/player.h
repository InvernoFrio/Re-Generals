#ifndef _PLAYER_H_
#define _PLAYER_H_

#include"raylib/raylib.h"
#include"controller.h"

class Player : public Controller {
private:
    bool is_dragging;
    int last_screen_width, last_screen_height;
    Vector2 camera_last_pos;
    Camera2D camera;
public:
    Player() = default;
    Player(int id, Color color);
    void update()override;
    void initCamera();
    void updateCamera();
    void updateCameraOffset();
    void handleCameraInput();
    Camera2D getCamera();
};

#endif