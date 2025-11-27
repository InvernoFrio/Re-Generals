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
    size_t datasize;
};
struct GameStartMessage { // type 3 game start message has no additional data
    int type;
    int id;
    Pos player_general_pos;
};
struct GameStateMessage { // type 4 game state message has no additional data
    int type;
    int state; // 0: waiting, 1: running
};
struct MapInquiryMessage { // type 5 map inquiry message has no additional data
    int type;
};
struct GameStateInquiryMessage { // type 6 game state inquiry message has no additional data
    int type;
};
struct GameEndMessage { // type 7 game end message has no additional data
    int type;
    int winner_id;
};

#endif // _MESSAGE_H_