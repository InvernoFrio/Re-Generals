#include"texturepool.h"

Texture2D TexturePool::load(const int& key, const std::string& file_path) {
    Texture2D texture = LoadTexture(file_path.c_str());
    textures[key] = texture;
    auto it = textures.find(key);
    return texture;
}
Texture2D TexturePool::get(const int& key) {
    auto it = textures.find(key);
    if (it != textures.end()) {
        return it->second;
    }
    else return { 0 };
}
void TexturePool::init() {
    for (int i = 0;i < TEXTURE_NUMBER;i++) {
        load(i, TEXTURE_PATH[i]);
    }
}
void TexturePool::unloadAll() {
    for (auto& it : textures) {
        UnloadTexture(it.second);
    }
    textures.clear();
}