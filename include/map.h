#include"square.h"
#include"constants.h"
#ifndef _MAP_H_
#define _MAP_H_

class Map {
private:
    int MAP_HEIGHT, MAP_WIDTH;
    Square map[DEFAULT_MAP_HEIGHT][DEFAULT_MAP_WIDTH];
public:
    void drawMap();
};


#endif