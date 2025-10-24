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

constexpr int DEFAULT_MOUNTAIN_NUMBER = 20;
constexpr int DEFAULT_CITY_NUMBER = 20;

constexpr int SPECTATOR = 0;

constexpr Color BACKGUROND_COLOR = Color{ 34,34,34,255 };
constexpr Color FOG_COLOR = Color{ 57,57,57,255 };
constexpr Color DEFAULT_COLOR_LAND = Color{ 220,220,220,255 };
constexpr Color DEFAULT_COLOR_MOUNTAIN = Color{ 187,187,187,255 };
constexpr Color DEFAULT_COLOR_CITY = Color{ 128,128,128,255 };


enum SquareType {
    TYPE_GENERAL,
    TYPE_CITY,
    TYPE_MOUNTAIN,
    TYPE_LAND,
};

constexpr float FONT_SIZE = 40;

constexpr Color color_pool[12] = { (Color) { 250,2,9,255 },(Color) { 68,144,253,255 }, (Color) { 8,128,12 ,255 },(Color) { 26,128,128,255 },(Color) { 245,141,21,255 },(Color) { 238,44,228 ,255 },(Color) { 127,0,127 ,255 },(Color) { 152,1,4 ,255 },(Color) { 176,173,56 ,255 },(Color) { 151,94,39 ,255 },(Color) { 57,40,253 ,255 },(Color) { 93,74,164 ,255 } };

#endif