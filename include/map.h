#ifndef MAP_H
#define MAP_H

// #include"raylib/raylib.h"
#include"constants.h"
#include<iostream>
#include<chrono>

struct Square {
    int id;
    Color color;
    int num;
    int type;
};
struct MapDataHeader {
    int type;
    int height;
    int width;
    size_t datasize;
    std::chrono::high_resolution_clock::time_point timestamp;
};
class Map {
private:
    Square data[DEFAULT_MAP_HEIGHT][DEFAULT_MAP_WIDTH];
    int height, width;
public:
    void init(int player_number = DEFAULT_PLAYER_NUMBER, int mountain_number = DEFAULT_MOUNTAIN_NUMBER, int city_number = DEFAULT_CITY_NUMBER);
    Square& getSquare(int x, int y) { return data[x][y]; }
    void setHeight(int h) { height = h; }
    void setWidth(int w) { width = w; }
    int getHeight() { return height; }
    int getWidth() { return width; }
    Square* getData() { return &data[0][0]; }
};



#endif // MAP_H