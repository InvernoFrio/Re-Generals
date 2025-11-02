#include"controller.h"

Color Controller::getColor() { return this->color; }
int Controller::getId() { return this->id; }
int Controller::getLandNum() { return this->land_num; }
void Controller::setId(int id) { this->id = id; }
void Controller::setColor(Color color) { this->color = color; }
int Controller::getSolderNum() { return this->solder_num; }
void Controller::setSolderNum(int solder_num) { this->solder_num = solder_num; }
void Controller::kill() {
    this->is_alive = false;
    this->land_num = 0;
    this->solder_num = 0;
    clearMotionQueue();
}