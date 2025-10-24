#include"map.h"
#include<random>
#include<time.h>

int dx[] = { -1,-1,-1,0,0,0,1,1,1 };
int dy[] = { -1,0,1,-1,0,1,-1,0,1 };

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

    //init texture_pool
    textures_pool.init();

    //init squares
    for (int i = 1;i <= map_height;i++) {
        for (int j = 1;j <= map_width;j++) {
            Square& now = map[i][j];
            now.setX((i - 1) * SQUARE_SIZE);
            now.setY((j - 1) * SQUARE_SIZE);
            now.setSpawnSpeed(INF);
            now.setSquareType(TYPE_LAND);
            now.setSolderNum(0);
            now.setColor(DEFAULT_COLOR);
            now.setId(0);
        }
    }

    //generate obstacles()
    do {
        generateMountains();
    } while (!checkConnectivity());
    do {
        generateCities();
    } while (!checkConnectivity());

}

void Map::drawMap(Controller& spectator) {
    for (int i = 1;i <= map_height;i++) {
        for (int j = 1;j <= map_width;j++) {
            Square& now = map[i][j];
            bool is_discovered = false;
            for (int k = 0;k < 9;k++) {
                int tox = i + dx[k], toy = j + dy[k];
                if (tox<1 || tox>map_height || toy<1 || toy>map_width)continue;
                if (map[tox][toy].getId() == spectator.getId()) is_discovered = true;
            }
            Rectangle rec = { now.getX(), now.getY(), SQUARE_SIZE, SQUARE_SIZE };
            if (is_discovered || spectator.getId() == 0) {
                DrawRectangleRec(rec, now.getColor());
                if (now.getType() != TYPE_LAND)
                    DrawTexture(textures_pool.get(now.getType()), now.getX(), now.getY(), WHITE);
            }
            else {
                DrawRectangleRec(rec, brightness(now.getColor(), 0.5f));
                if (now.getType() == TYPE_CITY || now.getType() == TYPE_MOUNTAIN)
                    DrawTexture(textures_pool.get(OBSTACLE), now.getX(), now.getY(), WHITE);
            }
            int line_thickness = std::min(5 / spectator.getCamera().zoom, 5.0f);
            DrawRectangleLinesEx(rec, line_thickness, BLACK);
        }
    }
}

void Map::generateMountains() {
    srand(time(0));

}
void Map::generateCities() {

}
bool Map::checkConnectivity() {
    return true;
}
Color Map::brightness(Color base_color, float factor) {
    Color tint;
    tint.r = base_color.r * factor;
    tint.g = base_color.g * factor;
    tint.b = base_color.b * factor;
    tint.a = base_color.a;
    return tint;
}