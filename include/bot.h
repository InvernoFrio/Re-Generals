#ifndef _BOT_H_
#define _BOT_H

#include"raylib/raylib.h"
#include"controller.h"

class Bot :public Controller {
public:
    Bot(int id, Color, Square(*map)[MAX_MAP_WIDTH + 1]);
    void update() override;
    void clearMotionQueue()override;
    Pos getMotion()override;
};

#endif