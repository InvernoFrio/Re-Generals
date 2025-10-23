#include "raylib/raylib.h"
#include "constants.h"
#ifndef _SQUARE_H_
#define _SQUARE_H_
class Square {
    int x, y;
    int num_solders, speed_spawn;
    int type;
    int home_id;
    Color color;

public:
    Square();
    int getX();
    int getY();
    int getNumSolders();
    int getSpeedSpawn();
    void setX(int x);
    void setY(int y);
    void setSpeedSpawn(int speed_spawn);
    void setSquareType(int type);
    void updateSquare();
};
#endif