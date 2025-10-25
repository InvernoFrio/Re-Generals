#ifndef _GAME_H_
#define _GAME_H_
#include"raylib/raylib.h"
#include"bits/unique_ptr.h"
#include"raylib/raymath.h"
#include"map.h"
#include"player.h"
#include"bot.h"
#include"texturepool.h"
#include"constants.h"
#include<time.h>
#include<vector>
#include<unordered_map>
class Game {
private:
    bool color_selected[MAX_PLAYER_NUMBER];
    int player_number, bot_number;
    bool is_running;

    Map map;
    Player* activated_player_ptr;
    std::vector < std::unique_ptr<Controller> >controller_pool;
    std::unordered_map<int, Player* >player_register;

public:
    //game basic
    void init();
    void update();
    void render();
    void run();
    void cleanup();
    void setPlayerNumber(int player_number);
    void setBotNumber(int bot_number);
    void setActivatedPlayer(int id);
    void updateEconomy();

    //map
    void initMap();
    void generateObstacles();
    void generateMountains();
    void generateCities();
    void generateGeneral();
    void refreshMap();
    bool checkMapConnectivity();
    void clearMap();

    //controller
    void initControllerPool();
    void updateControllers();

    Color selectColor();
};
#endif