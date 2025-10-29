#ifndef _SQUARE_H_
#define _SQUARE_H_

#include "raylib/raylib.h"
#include "constants.h"
#include<unordered_map>

class Square {
    int x, y;
    std::unordered_map<int, bool>selectedby;
    std::unordered_map<int, int>aroundedby;
    std::unordered_map<int, int>arrow[MAX_PLAYER_NUMBER + 1];
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
    void setSelectState(int id, bool select_state);
    void setAroundedState(int id, int arrow);
    bool isSelected(int id);
    int aroundedBy(int id);
    void setArrow(int id, int direction, bool f);
    int getArrowState(int id, int direction);
};
#endif