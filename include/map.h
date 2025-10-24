#include"square.h"
#include"texturepool.h"
#include"constants.h"
#include"controller.h"
#include<algorithm>
#include<iostream>
#include<string.h>
#ifndef _MAP_H_
#define _MAP_H_

class Map {
private:
    Font font;
    int map_height, map_width;
    int city_number, mountain_number;

public:
    Square map[DEFAULT_MAP_HEIGHT + 1][DEFAULT_MAP_WIDTH + 1];
    TexturePool textures_pool;
    bool setHeight(int map_height);
    bool setWidth(int map_width);
    bool setCityNumber(int city_number);
    bool setMountainNumber(int mountian_number);
    int getHeight();
    int getWidth();
    int getCityNumber();
    int getMountainNumber();

    Color brightness(Color base_color, float factor);
    void init();
    void draw(Controller& spectator);
    void drawScaledTexture(Texture2D texture, Rectangle square);
    void drawCentredText(const char* text, Rectangle square, float factor);
};


#endif