#include"map.h"
#include<random>

void Map::init(int player_number, int mountain_number, int city_number) {
    // 初始化地图为陆地
    std::cout << "Initializing map with "
        << player_number << " players, "
        << mountain_number << " mountains, "
        << city_number << " cities." << std::endl;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            data[i][j].id = 0;
            data[i][j].num = 0;
            data[i][j].type = TYPE_LAND; // 陆地
        }
    }

    // 随机生成山脉
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_height(0, height - 1);
    std::uniform_int_distribution<> dis_width(0, width - 1);

    for (int m = 0; m < mountain_number; m++) {
        int x, y;
        do {
            x = dis_height(gen);
            y = dis_width(gen);
        } while (data[x][y].type != TYPE_LAND); // 确保该位置是陆地
        data[x][y].type = TYPE_MOUNTAIN; // 山脉
    }

    // 随机生成城市
    for (int c = 0; c < city_number; c++) {
        int x, y;
        do {
            x = dis_height(gen);
            y = dis_width(gen);
        } while (data[x][y].type != TYPE_LAND); // 确保该位置是陆地
        data[x][y].type = TYPE_CITY;// 城市
    }

    // 随机生成将军位置
    for (int p = 1; p <= player_number; p++) {
        int x, y;
        do {
            x = dis_height(gen);
            y = dis_width(gen);
        } while (data[x][y].type != TYPE_LAND); // 确保该位置是陆地
        data[x][y].type = TYPE_GENERAL; // 将军
        data[x][y].id = p;   // 玩家ID
        data[x][y].num = 1;  // 初始士兵数
    }
}