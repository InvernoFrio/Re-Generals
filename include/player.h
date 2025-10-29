#ifndef _PLAYER_H_
#define _PLAYER_H_

#include"raylib/raylib.h"
#include"controller.h"
#include<time.h>
#include<deque>

struct Pos {
    int x, y, dir;
    Pos(int x = 0, int y = 0, int dir = 0) :x(x), y(y), dir(dir) {}
};

class Player : public Controller {
private:
    bool is_dragging;
    int last_screen_width, last_screen_height;
    int map_height, map_width;
    Pos last_select_pos;
    Vector2 camera_last_pos;
    Camera2D camera;
    std::deque<Pos>motion_queue;
    clock_t timer;
public:
    Player() = default;
    Player(int id, Color color, Square(*map)[MAX_MAP_WIDTH + 1]);
    void update()override;
    void initCamera();
    void updateCamera();
    void updateCameraOffset();
    void handleCameraInput();
    Camera2D getCamera();
    void handleMouseInput();
    void handleKBInput();
    void updateSelectedSquare(int x, int y, bool flag);
    void setMapHeight(int map_height) { this->map_height = map_height; }
    void setMapWidth(int map_width) { this->map_width = map_width; }
    bool checkMotionQueueLimit();
    void clearMotionQueue();
    void undoMotionQueue();
    void resetCamera();
};

#endif