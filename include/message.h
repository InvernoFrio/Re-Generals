#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <chrono>
#include <cstddef>
#include"constants.h"

struct MapDataHeader { //type 1
    int type;
    int height;
    int width;
    size_t datasize;
    std::chrono::high_resolution_clock::time_point timestamp;
};
struct MovementHeader { //type 2
    int type;
    int id;
    int x;
    int y;
    int dir;
};
struct GameStartMessage { // type 3 game start message has no additional data
    int type;
    int id;
    Pos player_general_pos;
};
struct MapInquiryMessage { // type 4 map inquiry message has no additional data
    int type;
};

#endif // _MESSAGE_H_