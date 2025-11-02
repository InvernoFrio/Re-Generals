#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include"raylib/raylib.h"
#include"raylib/raymath.h"
#include"square.h"
#include"constants.h"
#include<iostream>
struct Pos {
    int x, y, dir;
    Pos(int x = 0, int y = 0, int dir = 0) :x(x), y(y), dir(dir) {}
};
class Controller {
protected:
    int id;
    int land_num, solder_num;
    bool is_alive;
    Color color;
    Square(*map)[MAX_MAP_WIDTH + 1];
    int map_height, map_width;

public:
    virtual ~Controller() = default;
    int getId();
    void setId(int id);
    void setColor(Color color);
    int getLandNum();
    Color getColor();
    virtual void update() = 0;
    virtual Pos getMotion() = 0;
    virtual void clearMotionQueue() = 0;
    void setLandNum(int land_num) { this->land_num = land_num; }
    bool isAlive() { return is_alive; }
    void kill();
    void setMapHeight(int map_height) { this->map_height = map_height; }
    void setMapWidth(int map_width) { this->map_width = map_width; }
    int getSolderNum();
    void setSolderNum(int solder_num);
};

#endif