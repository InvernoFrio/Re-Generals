#ifndef _TEXTUREPOOL_H_
#define _TEXTUREPOOL_H_

#include<map>
#include<string>
#include<stdexcept>
#include"raylib/raylib.h"
#include"constants.h"


class TexturePool {
    std::map<int, Texture2D>textures;
public:
    Texture2D load(const int& key, const std::string& file_path);
    Texture2D get(const int& key);
    void unloadAll();
    void init();
};

#endif