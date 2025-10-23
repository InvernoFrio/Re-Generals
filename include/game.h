#ifndef _GAME_H_
#define _GAME_H_
#include"raylib/raylib.h"
#include"raylib/raymath.h"
#include"map.h"
#include"controller.h"
#include"texturepool.h"
#include"constants.h"
#include<vector>
class Game {
private:
    int player_number;
    bool is_running;

    Map map;
    std::vector<Controller>controller_pool;
    Controller main_contoller;

    TexturePool textures_pool;

public:
    void init();
    void update();
    void render();
    void run();
    void cleanup();
    void setMainController(Controller& controller);
    void setPlayerNumber(int player_number);
    Color selectColor();
};
#endif