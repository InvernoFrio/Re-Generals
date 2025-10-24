#ifndef _GAME_H_
#define _GAME_H_
#include"raylib/raylib.h"
#include"bits/unique_ptr.h"
#include"raylib/raymath.h"
#include"map.h"
#include"player.h"
#include"controller.h"
#include"texturepool.h"
#include"constants.h"
#include<time.h>
#include<vector>
class Game {
private:
    bool color_selected[MAX_PLAYER_NUMBER];
    int player_number;
    bool is_running;

    Map map;
    Controller spectator;
    std::vector < std::unique_ptr<Controller> >controller_pool;

public:
    //game basic
    void init();
    void update();
    void render();
    void run();
    void cleanup();
    void setSpectator(Controller controller);
    void setPlayerNumber(int player_number);
    void updateEconomy();

    //map
    void initMap();
    void generateObstacles();
    void generateMountains();
    void generateCities();
    void generateGeneral();
    void refreshMap();
    bool checkConnectivity();
    void clearMap();

    //controller
    void initController();
    void updateController();
    void updateSpectator();
    void updateKBInput();

    Color selectColor(int id);
};
#endif