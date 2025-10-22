#include "raylib/raylib.h"
#ifndef _SQUARE_H_
#define _SQUARE_H_
enum SquareType {
    SQUARE_TYPE_GENERAL,
    SQUARE_TYPE_CITY,
    SQUARE_TYPE_MOUNTAIN,
    SQUARE_TYPE_LAND,
};
enum RenderType {
    RENDER_TYPE_LAND,
    RENDER_TYPE_FOG,
    RENDER_TYPE_FOG_OBSTACLE
};
class Square {
    int x, y;
    int num_solders, speed_spawn;
    int square_type, render_type;
    int home_id;
    Color color;

public:
    int getX();
    int getY();
    int getNumSolders();
    int getSpeedSpawn();
    void setX(int x);
    void setY(int y);
    void setSpeedSpawn(int speed_spawn);
    void setSquareType(int square_type);
    void setRenderType(int render_type);
    void updateSquare();
};
#endif