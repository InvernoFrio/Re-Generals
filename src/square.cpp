#include"square.h"

Square::Square() :type(SQUARE_TYPE_LAND), color(WHITE) {}

int Square::getX() { return this->x; }
int Square::getY() { return this->y; }
int Square::getNumSolders() { return this->num_solders; }
int Square::getSpeedSpawn() { return this->speed_spawn; }
void Square::setX(int x) { this->x = x; }
void Square::setY(int y) { this->y = y; }
void Square::setSpeedSpawn(int speed_spawn) { this->speed_spawn = speed_spawn; }
void Square::setSquareType(int type) { this->type = type; }
void Square::updateSquare() {

}