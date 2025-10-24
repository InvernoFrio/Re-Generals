#include"square.h"
#include"texturepool.h"
#include"constants.h"
#include"controller.h"
#include<algorithm>
#include<iostream>
#ifndef _MAP_H_
#define _MAP_H_

class Map {
private:
    int map_height, map_width;
    Square map[DEFAULT_MAP_HEIGHT + 1][DEFAULT_MAP_WIDTH + 1];

public:
    TexturePool textures_pool;
    bool setMapHeight(int map_height);
    bool setMapWidth(int map_width);
    Color brightness(Color base_color, float factor);
    void init();
    void drawMap(Controller& spectator);
    void generateMountains();
    void generateCities();
    bool checkConnectivity();
};


#endif