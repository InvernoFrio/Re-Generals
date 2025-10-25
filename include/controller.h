#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include"raylib/raylib.h"
#include"raylib/raymath.h"
#include<stack>
class Controller {
protected:
    int id;
    int land_num;
    bool is_alive;
    Color color;

public:
    virtual ~Controller() = default;
    int getId();
    void setId(int id);
    void setColor(Color color);
    int getLandNum();
    Color getColor();
    virtual void update() = 0;
};

#endif