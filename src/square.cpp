#include"square.h"

void Square::setColor(Color color) { this->color = color; }
Color Square::getColor() { return this->color; }
int Square::getX() { return this->x; }
int Square::getY() { return this->y; }
int Square::getNumSolders() { return this->solder_num; }
int Square::getId() { return this->id; }
int Square::getType() { return this->type; }
void Square::setX(int x) { this->x = x; }
void Square::setY(int y) { this->y = y; }
void Square::setType(int type) { this->type = type; }
void Square::setSolderNum(int solder_num) { this->solder_num = solder_num; }
void Square::setId(int id) { this->id = id; }
void Square::grow() { this->solder_num += 1; }