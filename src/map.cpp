#include"map.h"
#include<random>
#include<time.h>

Vector2 getWorldPos(int x, int y) {
    return (Vector2) { (x - 1)* SQUARE_SIZE, (y - 1)* SQUARE_SIZE };
}
bool Map::setMapHeight(int map_height) {
    if (map_height <= MAX_MAP_HEIGHT)this->map_height = map_height;
    else return 0;
    return 1;
}
bool Map::setMapWidth(int map_width) {
    if (map_width <= MAX_MAP_WIDTH)this->map_width = map_width;
    else return 0;
    return 1;
}

void Map::init() {
    setMapHeight(DEFAULT_MAP_HEIGHT);
    setMapWidth(DEFAULT_MAP_WIDTH);

    while (checkConnectivity())generateObstacles();
}

void Map::drawMap(Controller& main_controller) {
    for (int i = 1;i <= map_height;i++) {
        for (int j = 1;j <= map_width;j++) {

        }
    }
}

void Map::generateObstacles() {
    srand(time(0));
}
bool Map::checkConnectivity() {
    return true;
}