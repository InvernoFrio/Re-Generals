#ifndef _SERVER_H_
#define _SERVER_H_

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include<atomic>
#include "map.h"

class Server {
private:
    SOCKET server_socket;
    WSADATA wsa_data;
    Map map;
    std::vector<std::thread> client_threads;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    std::thread game_thread;
    std::mutex client_threads_mutex;
    std::atomic<bool> running;
    std::vector<Movement> movements;

public:
    Server() : server_socket(INVALID_SOCKET) {}

    bool initialize();
    bool start(int port);
    void run();
    void handle_client(SOCKET client_socket, sockaddr_in client_addr);

    void startGame();
    void updateGame();

    ~Server();
};

#endif