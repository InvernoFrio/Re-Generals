#ifndef RENDER_H
#define RENDER_H
#include"raylib/raylib.h"
#include"raylib/raymath.h"
#include"constants.h"
#include"map.h"
#include<mutex>
#include<atomic>
#include<algorithm>
#include<map>



class Render {
private:
    int id;
public:
    Camera2D camera;
    int last_screen_height, last_screen_width;
    bool is_dragging;
    Vector2 camera_last_pos;
    Map* map_ptr;
    std::mutex mtx;
    std::map<int, Texture2D>textures;
    Font font;
    Pos selected_square{ -1,-1 };

public:
    void init(int id, Map* map_ptr);
    void draw();
    void drawUI();
    void drawMap();
    void initCamera();

    void handleCameraInput();
    void resetCamera();

    void updateCamera();

    void updateCameraOffset();
    void drawScaledTexture(Texture2D texture, Rectangle square);
    void drawCentredText(const char* text, Rectangle square, float factor);
    Color brightness(Color base_color, float factor);
    ~Render();
};

#endif // RENDER_H