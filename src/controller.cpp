#include"controller.h"

Color Controller::getColor() { return this->color; }
int Controller::getId() { return this->id; }
int Controller::getLandNum() { return this->land_num; }
void Controller::setId(int id) { this->id = id; }
void Controller::setColor(Color color) { this->color = color; }