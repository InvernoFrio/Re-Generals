#ifndef MAP_H
#define MAP_H

// #include"raylib/raylib.h"
#include"constants.h"

struct Square {
    int id;
    // Color color;
    short num;
};
class Map {
private:
    Square data[DEFAULT_MAP_HEIGHT][DEFAULT_MAP_WIDTH];
    int height, width;
public:
    void init();
    Square& getSquare(int x, int y) { return data[x][y]; }
    void setHeight(int h) { height = h; }
    void setWidth(int w) { width = w; }
};



#endif // MAP_H