#include"map.h"
#include<random>
#include<time.h>

int dx[] = { -1,-1,-1,0,0,0,1,1,1 };
int dy[] = { -1,0,1,-1,0,1,-1,0,1 };


void Map::init() {
    //init texture_pool
    texture_pool.init();
    //init font
    font = LoadFont("./assets/LiberationSans.ttf");
}
void Map::draw(Player* player, int map_height, int map_width) {
    for (int i = 1;i <= map_height;i++) {
        for (int j = 1;j <= map_width;j++) {
            int player_id = player->getId();
            Square& now = map[i][j];
            //check is_discovered
            bool is_discovered = false;
            for (int k = 0;k < 9;k++) {
                int tox = i + dx[k], toy = j + dy[k];
                if (tox<1 || tox>map_height || toy<1 || toy>map_width)continue;
                if (map[tox][toy].getId() == player_id) is_discovered = true;
            }


            Rectangle rec = { (float)now.getX(), (float)now.getY(), (float)SQUARE_SIZE, (float)SQUARE_SIZE };
            int line_thickness = std::min(static_cast<float>(5) / player->getCamera().zoom, 3.0f);
            //draw squares
            if (is_discovered || player_id == SPECTATOR) {//discovered
                Color color = brightness(now.getColor(), map[i][j].aroundedBy(player_id) ? 0.5f : 1.0f);//set color if arounded

                //draw rectangle&texture
                DrawRectangleRec(rec, color);
                if (now.getType() != TYPE_LAND)drawScaledTexture(texture_pool.get(now.getType()), rec);

                //draw lines
                DrawRectangleLinesEx(rec, line_thickness, (map[i][j].isSelected(player_id) ? WHITE : BLACK));
                if (now.getNumSolders() != 0)drawCentredText((std::to_string(now.getNumSolders())).c_str(), rec, player->getCamera().zoom);
            }
            else {//undiscovered
                Color color = brightness(FOG_COLOR, (map[i][j].aroundedBy(player_id) ? 0.5f : 1.0f));//set color if arounded
                DrawRectangleRec(rec, color);
                if (now.getType() == TYPE_CITY || now.getType() == TYPE_MOUNTAIN)//draw texture
                    drawScaledTexture(texture_pool.get(OBSTACLE), rec);
                if (map[i][j].isSelected(player_id))//draw lines if selected
                    DrawRectangleLinesEx(rec, line_thickness, WHITE);
            }

            //draw arrow
            for (int arrow = ARROWUP;arrow <= ARROWRIGHT;arrow++) {
                if (map[i][j].getArrowState(player_id, arrow)) {
                    DrawTexture(texture_pool.get(arrow), rec.x, rec.y, WHITE);
                }
            }
        }
    }
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

    Rectangle source_rec = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
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