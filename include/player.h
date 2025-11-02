#ifndef _PLAYER_H_
#define _PLAYER_H_

#include"raylib/raylib.h"
#include"controller.h"
#include<time.h>
#include<deque>

class Player : public Controller {
private:
    bool is_dragging;
    int last_screen_width, last_screen_height;

    Pos last_select_pos;
    Vector2 camera_last_pos;
    Camera2D camera;
    std::deque<Pos>motion_queue;
    clock_t timer;
public:
    Player() = default;
    Player(int id, Color color, Square(*map)[MAX_MAP_WIDTH + 1]);
    void update()override;
    Pos getMotion()override;
    void initCamera();
    void updateCamera();
    void updateCameraOffset();
    void handleCameraInput();
    Camera2D getCamera();
    void handleMouseInput();
    void handleKBInput();
    void updateSelectedSquare(int x, int y, bool flag);
    bool checkMotionQueueLimit();
    void clearMotionQueue()override;
    void undoMotionQueue();
    void resetCamera();
};

#endif