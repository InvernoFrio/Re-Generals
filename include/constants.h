#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

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
    "./assets/arrowleft",
    "./assets/arrowright"
};

constexpr int MAX_PLAYER_NUMBER = 12;
constexpr int MAX_MAP_HEIGHT = 100;
constexpr int MAX_MAP_WIDTH = 100;

constexpr int SQUARE_SIZE = 100;

constexpr int DEFAULT_SCREEN_WIDTH = 800;
constexpr int DEFAULT_SCREEN_HEIGHT = 600;
constexpr int DEFAULT_FPS = 60;
constexpr int DEFAULT_PLAYER_NUMBER = 8;
constexpr int DEFAULT_MAP_WIDTH = 30;
constexpr int DEFAULT_MAP_HEIGHT = 30;

constexpr int OBSTACLE_NUMBER = 15;

const Color BACKGUROND_COLOR = Color{ 34,34,34,255 };
enum SquareType {
    SQUARE_TYPE_LAND,
    SQUARE_TYPE_GENERAL,
    SQUARE_TYPE_CITY,
    SQUARE_TYPE_MOUNTAIN,
};
enum RenderType {
    RENDER_TYPE_LAND,
    RENDER_TYPE_FOG,
    RENDER_TYPE_FOG_OBSTACLE
};



#endif