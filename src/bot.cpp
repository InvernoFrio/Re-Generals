#include"bot.h"
#include"raylib/raylib.h"

Bot::Bot(int id, Color color, Square(*map)[MAX_MAP_WIDTH + 1]) {
    this->setId(id);
    this->setColor(color);
    this->map = map;
    is_alive = true;
    land_num = 1;
    solder_num = 1;
}
void Bot::update() {
}
void Bot::clearMotionQueue() {

}
Pos Bot::getMotion() {
    return Pos(0, 0, NO_ARROW);
}