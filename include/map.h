#include"square.h"
#include"constants.h"
#include"controller.h"
#ifndef _MAP_H_
#define _MAP_H_

class Map {
private:
    int map_height, map_width;
    Square map[DEFAULT_MAP_HEIGHT][DEFAULT_MAP_WIDTH];
public:
    bool setMapHeight(int map_height);
    bool setMapWidth(int map_width);
    void init();
    void drawMap(Controller& main_controller);
    void generateObstacles();
    bool checkConnectivity();
};


#endif