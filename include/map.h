#include"square.h"
#include"texturepool.h"
#include"constants.h"
#include"player.h"
#include<algorithm>
#include<iostream>
#include<string.h>
#ifndef _MAP_H_
#define _MAP_H_

class Map {
private:
    Font font;
public:
    TexturePool texture_pool;

    Square map[MAX_MAP_HEIGHT + 1][MAX_MAP_WIDTH + 1];
    Color brightness(Color base_color, float factor);
    void init();
    void draw(Player* spectator, int map_height, int map_width);
    void drawScaledTexture(Texture2D texture, Rectangle square);
    void drawCentredText(const char* text, Rectangle square, float factor);
};


#endif