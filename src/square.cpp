#include"square.h"

int Square::getX() { return this->x; }
int Square::getY() { return this->y; }
int Square::getNumSolders() { return this->num_solders; }
int Square::getSpeedSpawn() { return this->speed_spawn; }
void Square::setX(int x) { this->x = x; }
void Square::setY(int y) { this->y = y; }
void Square::setSpeedSpawn(int speed_spawn) { this->speed_spawn = speed_spawn; }
void Square::setSquareType(int square_type) { this->square_type = square_type; }
void Square::setRenderType(int render_type) { this->render_type = render_type; }
void Square::updateSquare() {

}