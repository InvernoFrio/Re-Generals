#include"map.h"
#include<random>
#include<time.h>

int dx[] = { -1,-1,-1,0,0,0,1,1,1 };
int dy[] = { -1,0,1,-1,0,1,-1,0,1 };

bool Map::setHeight(int map_height) {
    if (map_height <= MAX_MAP_HEIGHT)
        this->map_height = map_height;
    else return 0;
    return 1;
}
bool Map::setWidth(int map_width) {
    if (map_width <= MAX_MAP_WIDTH)
        this->map_width = map_width;
    else return 0;
    return 1;
}
int Map::getHeight() { return this->map_height; }
int Map::getWidth() { return this->map_width; }
int Map::getCityNumber() { return this->city_number; }
int Map::getMountainNumber() { return this->mountain_number; }

void Map::init() {
    //init texture_pool
    textures_pool.init();

    //init font
    font = LoadFont("./assets/LiberationSans.ttf");

    setHeight(DEFAULT_MAP_HEIGHT);
    setWidth(DEFAULT_MAP_WIDTH);
    setCityNumber(DEFAULT_CITY_NUMBER);
    setMountainNumber(DEFAULT_MOUNTAIN_NUMBER);
}
void Map::draw(Player& player) {
    for (int i = 1;i <= map_height;i++) {
        for (int j = 1;j <= map_width;j++) {
            Square& now = map[i][j];
            //check is_discovered
            bool is_discovered = false;
            for (int k = 0;k < 9;k++) {
                int tox = i + dx[k], toy = j + dy[k];
                if (tox<1 || tox>map_height || toy<1 || toy>map_width)continue;
                if (map[tox][toy].getId() == player.getId()) is_discovered = true;
            }
            Rectangle rec = { now.getX(), now.getY(), SQUARE_SIZE, SQUARE_SIZE };
            //draw squares
            if (is_discovered || player.getId() == 0) {
                DrawRectangleRec(rec, now.getColor());
                if (now.getType() != TYPE_LAND)drawScaledTexture(textures_pool.get(now.getType()), rec);
                int line_thickness = std::min(5 / player.getCamera().zoom, 3.0f);
                DrawRectangleLinesEx(rec, line_thickness, BLACK);
                if (now.getNumSolders() != 0)drawCentredText((std::to_string(now.getNumSolders())).c_str(), rec, player.getCamera().zoom);
            }
            else {
                DrawRectangleRec(rec, FOG_COLOR);
                if (now.getType() == TYPE_CITY || now.getType() == TYPE_MOUNTAIN)
                    drawScaledTexture(textures_pool.get(OBSTACLE), rec);
            }
        }
    }
}

bool Map::setCityNumber(int city_number) {
    this->city_number = city_number;
    return 1;
}
bool Map::setMountainNumber(int mountain_number) {
    this->mountain_number = mountain_number;
    return 1;
}

Color Map::brightness(Color base_color, float factor) {
    Color tint;
    tint.r = base_color.r * factor;
    tint.g = base_color.g * factor;
    tint.b = base_color.b * factor;
    tint.a = base_color.a;
    return tint;
}

void Map::drawScaledTexture(Texture2D texture, Rectangle square) {
    float factor = 0.8f;

    float posX = square.x + SQUARE_SIZE * (1 - factor) / 2;
    float posY = square.y + SQUARE_SIZE * (1 - factor) / 2;

    Rectangle source_rec = { 0,0,texture.height,texture.width };
    Rectangle dest_rec = { posX, posY, SQUARE_SIZE * factor, SQUARE_SIZE * factor };

    DrawTexturePro(texture, source_rec, dest_rec, { 0,0 }, 0.0f, WHITE);
}
void Map::drawCentredText(const char* text, Rectangle square, float factor) {
    int font_size = std::min(50.0f, FONT_SIZE / factor);
    Vector2 text_size = MeasureTextEx(font, text, font_size, 0);

    float posX = square.x + (SQUARE_SIZE - text_size.x) / 2;
    float posY = square.y + (SQUARE_SIZE - text_size.y) / 2;
    float offset = 2.0f / factor;
    DrawTextEx(font, text, (Vector2) { posX + offset, posY + offset }, font_size, 0, { 0,0,0,127 });
    DrawTextEx(font, text, (Vector2) { posX, posY }, font_size, 0, WHITE);
}