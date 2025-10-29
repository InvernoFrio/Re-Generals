#include"game.h"
#include<memory>

int main() {
    auto game = std::make_unique<Game>();
    game->init();
    game->run();
    return 0;
}