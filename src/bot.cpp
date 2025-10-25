#include"bot.h"
#include"raylib/raylib.h"

Bot::Bot(int id, Color color) {
    this->setId(id);
    this->setColor(color);
    is_alive = true;
    land_num = 1;
}
void Bot::update() {
}