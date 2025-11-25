#ifndef _CLIENT_H_
#define _CLIENT_H_

#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER
#define NOKERNEL
#define NOSOUND
#define NOMINMAX
#include <winsock2.h>
#include <ws2tcpip.h>

#include"render.h"
#include "map.h"
#include"constants.h"
#include <iostream>
#include <string>
#include <thread>
#include <thread>
#include <mutex>
#include <deque>

class Client {
private:
    int id;
    SOCKET client_socket;
    WSADATA wsa_data;
    // Render render;
    Map map;
    std::thread network_thread;

    std::deque<Movement> movements;

public:
    Client() : client_socket(INVALID_SOCKET) {}
    bool initialize();

    bool connect_to_server(const std::string& host, int port);
    void run();
    void update();
    void recieveData(const void* data, int length);

    void handleNetwork();

    ~Client();
};

#endif