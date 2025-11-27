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

#include "map.h"
#include"constants.h"
#include <iostream>
#include <string>
#include <thread>
#include <thread>
#include <mutex>
#include <deque>
#include "render.h"
#include "message.h"

class Client {
private:
    int id;
    int speed = 2; // Updates per second
    SOCKET client_socket;
    WSADATA wsa_data;
    Map map;
    std::mutex map_lock;
    std::chrono::high_resolution_clock::time_point last_update_time;
    Pos general_pos;

    std::deque<Movement> movements;

public:
    std::atomic<int> state;
    std::thread network_thread;
    Client() : client_socket(INVALID_SOCKET) {}
    bool initialize();

    bool connect_to_server(const std::string& host, int port);
    void handleServerCommunication();
    void receiveStateMessage(char* buffer);
    void receiveType(int bytes_received, int& type, char* buffer, int& retFlag);
    void inqueryMapdata(int& retFlag);
    void inqueyGameState(bool& retFlag);
    void recieveMapData(const void* data, int length);
    void recieveStartData(const void* data, int length);

    Map* getMap();
    std::deque<Movement>* getMovements();
    int getId();

    ~Client();
};

#endif