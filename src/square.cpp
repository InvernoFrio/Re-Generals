#include"square.h"

void Square::setColor(Color color) { this->color = color; }
Color Square::getColor() { return this->color; }
int Square::getX() { return this->x; }
int Square::getY() { return this->y; }
int Square::getSolderNum() { return this->solder_num; }
int Square::getId() { return this->id; }
int Square::getType() { return this->type; }
void Square::setX(int x) { this->x = x; }
void Square::setY(int y) { this->y = y; }
void Square::setType(int type) { this->type = type; }
void Square::setSolderNum(int solder_num) { this->solder_num = solder_num; }
void Square::setId(int id) { this->id = id; }
void Square::grow() { this->solder_num += 1; }
void Square::setSelectState(int id, bool is_selected) {
    selectedby[id] = is_selected;
}
void Square::setAroundedState(int id, int arrow) {
    aroundedby[id] = arrow;
}
bool Square::isSelected(int id) {
    return this->selectedby[id];
}
int Square::aroundedBy(int id) {
    return aroundedby[id];
}
void Square::setArrow(int id, int direction, bool f) { this->arrow[id][direction] += (f ? 1 : -1); }
int Square::getArrowState(int id, int direction) {
    return arrow[id][direction];
}