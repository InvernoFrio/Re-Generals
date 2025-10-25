#ifndef _BOT_H_
#define _BOT_H

#include"raylib/raylib.h"
#include"controller.h"

class Bot :public Controller {
public:
    Bot(int id, Color color);
    void update() override;
};

#endif