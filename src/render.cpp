#include "render.h"

void Render::init(int id, Map* map_ptr) {
    this->map_ptr = map_ptr;
    this->id = id;
    initCamera();
    InitWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, PROJECT_NAME);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    //load textures
    for (int i = 0;i < TEXTURE_NUMBER;i++) {
        Texture2D texture = LoadTexture(TEXTURE_PATH[i]);
        textures[i] = texture;
    }
    font = LoadFont("./assets/LiberationSans.ttf");
}
void Render::draw() {
    std::cout << "Render thread started." << std::endl;
    while (!WindowShouldClose()) {
        updateCamera();
        BeginDrawing();
        ClearBackground(BACKGUROND_COLOR);
        BeginMode2D(camera);
        drawMap();
        EndMode2D();
        drawUI();
        EndDrawing();
    }
}
void Render::updateCamera() {
    if (last_screen_height != GetScreenHeight() || last_screen_width != GetScreenWidth()) {
        updateCameraOffset();
    }
    handleCameraInput();
}
void Render::initCamera() {
    camera = { 0 };
    last_screen_width = GetScreenWidth();
    last_screen_height = GetScreenHeight();
    camera.target = (Vector2){ 0,0 };
    camera.offset = (Vector2){ 0,0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    is_dragging = false;
    updateCameraOffset();
}
void Render::updateCameraOffset() {
    camera.offset = (Vector2){
        GetScreenWidth() / 2.0f,
        GetScreenHeight() / 2.0f
    };
    last_screen_width = GetScreenWidth();
    last_screen_height = GetScreenHeight();
}
void Render::handleCameraInput() {
    float wheel = GetMouseWheelMove();

    if (wheel != 0) {
        Vector2 mouseWorldPosBefore = GetScreenToWorld2D(GetMousePosition(), camera);
        camera.zoom += wheel * 0.1f;
        camera.zoom = Clamp(camera.zoom, 0.3f, 3.0f);
        Vector2 mouseWorldPosAfter = GetScreenToWorld2D(GetMousePosition(), camera);

        camera.target.x -= (mouseWorldPosAfter.x - mouseWorldPosBefore.x);
        camera.target.y -= (mouseWorldPosAfter.y - mouseWorldPosBefore.y);
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        is_dragging = true;
        camera_last_pos = GetScreenToWorld2D(GetMousePosition(), camera);
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && is_dragging) {
        Vector2 camera_current_pos = GetScreenToWorld2D(GetMousePosition(), camera);
        camera.target.x -= (camera_current_pos.x - camera_last_pos.x);
        camera.target.y -= (camera_current_pos.y - camera_last_pos.y);
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        is_dragging = false;
    }

    if (IsKeyPressed(KEY_R)) {
        resetCamera();
    }
}
void Render::drawMap() {
    std::cout << "Drawing map..." << std::endl;
    std::lock_guard<std::mutex> lock(mtx);
    Map& data = *map_ptr;
    int height = data.getHeight();
    int width = data.getWidth();
    std::cout << "Map size: " << height << "x" << width << std::endl;
    for (int i = 0;i < height;i++) {
        for (int j = 0;j < width;j++) {
            Square& now = data.getSquare(i, j);
            bool is_selected = (selected_square.x == i && selected_square.y == j);
            bool is_discovered = false;
            for (int dx = -1;dx <= 1;dx++) {
                for (int dy = -1;dy <= 1;dy++) {
                    int tox = i + dx, toy = j + dy;
                    if (tox < 0 || tox >= height || toy < 0 || toy >= width)continue;
                    if (data.getSquare(tox, toy).id == id) is_discovered = true;
                }
            }

            float x = j * SQUARE_SIZE;
            float y = i * SQUARE_SIZE;
            Rectangle rec = { x, y, (float)SQUARE_SIZE, (float)SQUARE_SIZE };
            int line_thickness = std::min(static_cast<float>(5) / camera.zoom, 3.0f);
            //draw squares
            bool is_arounded = false;
            int dx[] = { 0,0,-1,1 };
            int dy[] = { -1,1,0,0 };
            for (int k = 0;k < 4;k++) {
                int tox = i + dx[k];
                int toy = j + dy[k];
                if (tox < 0 || tox >= height || toy < 0 || toy >= width)continue;
                if (tox == selected_square.x && toy == selected_square.y) {
                    is_arounded = true;
                    break;
                }
            }
            if (is_discovered || id == 0) {//discovered
                Color color = brightness(now.color, is_arounded ? 0.5f : 1.0f);//set color if arounded

                //draw rectangle&texture
                DrawRectangleRec(rec, now.color);
                if (now.type != TYPE_LAND)drawScaledTexture(textures[now.type], rec);

                //draw lines
                DrawRectangleLinesEx(rec, line_thickness, (is_selected ? WHITE : BLACK));
                if (now.num != 0)drawCentredText((std::to_string(now.num)).c_str(), rec, camera.zoom);
            }
            else {//undiscovered
                Color color = brightness(FOG_COLOR, is_arounded ? 0.5f : 1.0f);//set color if arounded
                DrawRectangleRec(rec, color);
                if (now.type == TYPE_CITY || now.type == TYPE_MOUNTAIN)//draw texture
                    drawScaledTexture(textures[OBSTACLE], rec);
                if (is_selected)//draw lines if selected
                    DrawRectangleLinesEx(rec, line_thickness, WHITE);
            }

            //draw arrow
        }
    }
    // Implement rendering logic here using the data
}
Color Render::brightness(Color base_color, float factor) {
    Color tint;
    tint.r = base_color.r * factor;
    tint.g = base_color.g * factor;
    tint.b = base_color.b * factor;
    tint.a = base_color.a;
    return tint;
}
void Render::drawScaledTexture(Texture2D texture, Rectangle square) {
    float factor = 0.8f;

    float posX = square.x + SQUARE_SIZE * (1 - factor) / 2;
    float posY = square.y + SQUARE_SIZE * (1 - factor) / 2;

    Rectangle source_rec = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
    Rectangle dest_rec = { posX, posY, SQUARE_SIZE * factor, SQUARE_SIZE * factor };

    DrawTexturePro(texture, source_rec, dest_rec, { 0,0 }, 0.0f, WHITE);
}
void Render::drawCentredText(const char* text, Rectangle square, float factor) {
    int font_size = std::min(50.0f, FONT_SIZE / factor);
    Vector2 text_size = MeasureTextEx(font, text, font_size, 0);

    float posX = square.x + (SQUARE_SIZE - text_size.x) / 2;
    float posY = square.y + (SQUARE_SIZE - text_size.y) / 2;
    float offset = 2.0f / factor;
    DrawTextEx(font, text, (Vector2) { posX + offset, posY + offset }, font_size, 0, { 0,0,0,127 });
    DrawTextEx(font, text, (Vector2) { posX, posY }, font_size, 0, WHITE);
}
void Render::drawUI() {
    std::cout << "Drawing UI..." << std::endl;
}
void Render::resetCamera() {
    camera.target = (Vector2){ 0,0 };
    camera.zoom = 1.0f;
    updateCameraOffset();
}
Render::~Render() {
    CloseWindow();
}