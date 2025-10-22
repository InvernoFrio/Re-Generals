#ifndef _TEXTUREPOOL_H_
#define _TEXTUREPOOL_H_

#include<vector>
#include"raylib/raylib.h"
#include"constants.h"


class TexturePool {
    std::vector<Texture2D>textures[MAX_PLAYER_NUMBER + 1];
    int texture_size;
};

#endif