#ifndef _SERVER_H_
#define _SERVER_H_

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include "map.h"
#include "constants.h"
#include "message.h"

class Server {
private:
    SOCKET server_socket;
    WSADATA wsa_data;
    struct ClientInfo {
        SOCKET sock = INVALID_SOCKET;
        sockaddr_in addr{};
        std::atomic<bool> alive{ false };
        std::thread th;
        int id;
    };

    std::atomic<int> state{ 0 }; // 0: waiting, 1: running, 2: ended

    std::vector<std::shared_ptr<ClientInfo>> clients;
    std::mutex client_threads_mutex;

    std::thread logic_thread;

    int rounds = 0;
    int speed = 2; // Updates per second
    Map map;
    std::vector<Movement> movements;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    void cleanup_clients();

public:
    Pos player_general_pos[MAX_PLAYER_NUMBER];
    Server() : server_socket(INVALID_SOCKET) {}

    bool initialize();
    bool start(int port);
    void run();
    void clear();
    void acceptClientConnections();
    void handle_client(SOCKET client_socket, sockaddr_in client_addr, int client_id);

    void sendGameStateEnded(SOCKET  client_socket, int client_id);

    void sendGameStartMessage(int client_id, SOCKET client_socket, int& retFlag);

    void sendMapData(int client_id, SOCKET client_socket);

    void sendGameStateWaiting(SOCKET client_socket, int& retFlag, int client_id);

    void handleInput(const char* data, int length, int client_id, int& type);

    void draw();

    void startGame();
    void getGeneralPos();
    void initMap();
    void updateGame();
    void* prepareMapdata(int& total_size);

    ~Server();
};

#endif