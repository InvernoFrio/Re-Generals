#ifndef _GAME_H_
#define _GAME_H_
#include"raylib/raylib.h"
#include"bits/unique_ptr.h"
#include"raylib/raymath.h"
#include"map.h"
#include"player.h"
#include"bot.h"
#include"texturepool.h"
#include<queue>
#include"constants.h"
#include<time.h>
#include<vector>
#include<unordered_map>
typedef std::pair<int, int> pii;
class Game {
private:
    bool color_selected[MAX_PLAYER_NUMBER];
    int player_number, bot_number;
    int frame_number, round;
    int speed;
    bool is_running;
    int map_height, map_width;
    int city_number, mountain_number;

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
    void setSpeed(int speed);

    //map
    void initMap();
    void generateObstacles();
    void generateMountains();
    void generateCities();
    void generateGeneral();
    void refreshMap();
    bool checkMapConnectivity();
    void clearMap();
    bool setHeight(int map_height);
    bool setWidth(int map_width);
    bool setCityNumber(int city_number);
    bool setMountainNumber(int mountian_number);

    //controller
    void initControllerPool();
    void updateControllers();

    Color selectColor();
};
#endif