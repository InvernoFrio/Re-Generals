#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

constexpr int INF = 0x3f3f3f3f;

constexpr char PROJECT_NAME[] = "Re-Generals";

constexpr int TEXTURE_NUMBER = 8;
enum TEXTURE_ID {
    GENERAL,
    CITY,
    MOUNTAIN,
    OBSTACLE,
    ARROWUP,
    ARROWDOWN,
    ARROWLEFT,
    ARROWRIGHT
};
constexpr char* TEXTURE_NAME[] = {
    "general",
    "city",
    "mountain",
    "obstacle",
    "arrowup",
    "arrowdown",
    "arrowleft",
    "arrowright"
};
constexpr char* TEXTURE_PATH[] = {
    "./assets/crown.png",
    "./assets/city.png",
    "./assets/mountain.png",
    "./assets/obstacle.png",
    "./assets/arrowup.png",
    "./assets/arrowdown.png",
    "./assets/arrowleft.png",
    "./assets/arrowright.png"
};

constexpr int MAX_PLAYER_NUMBER = 12;
constexpr int MAX_MAP_HEIGHT = 100;
constexpr int MAX_MAP_WIDTH = 100;

constexpr int SQUARE_SIZE = 100;

constexpr int DEFAULT_SCREEN_WIDTH = 800;
constexpr int DEFAULT_SCREEN_HEIGHT = 600;
constexpr int DEFAULT_FPS = 60;
constexpr int DEFAULT_PLAYER_NUMBER = 8;
constexpr int DEFAULT_MAP_WIDTH = 20;
constexpr int DEFAULT_MAP_HEIGHT = 20;

constexpr int MOUNTAIN_NUMBER = 20;
constexpr int CITY_NUMBER = 20;

constexpr int SPECTATOR = 0;

constexpr Color BACKGUROND_COLOR = Color{ 34,34,34,255 };
constexpr Color DEFAULT_COLOR = Color{ 220,220,220,255 };

enum SquareType {
    TYPE_GENERAL,
    TYPE_CITY,
    TYPE_MOUNTAIN,
    TYPE_LAND,
};




#endif