#include "raylib/raylib.h"
#include "constants.h"
#ifndef _SQUARE_H_
#define _SQUARE_H_
class Square {
    int x, y;
    int solder_num;
    int type;
    int id;
    Color color;

public:
    int getX();
    int getY();
    int getNumSolders();
    int getId();
    Color getColor();
    int getType();
    void setX(int x);
    void setY(int y);
    void setSolderNum(int solder_num);
    void grow();
    void setType(int type);
    void setColor(Color color);
    void setId(int id);
};
#endif