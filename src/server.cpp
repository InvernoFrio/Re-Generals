#include "server.h"

bool Server::initialize() {
    // 初始化 Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        return false;
    }
    running.store(false);
    return true;
}
bool Server::start(int port) {
    // 创建 socket
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        return false;
    }

    // 设置 SO_REUSEADDR
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR,
        (char*)&opt, sizeof(opt)) == SOCKET_ERROR) {
        std::cerr << "Setsockopt failed: " << WSAGetLastError() << std::endl;
    }

    // 绑定地址
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        return false;
    }

    // 开始监听
    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        return false;
    }

    std::cout << "Server listening on port " << port << std::endl;
    return true;
}
void Server::run() {
    while (true) {
        // 接受客户端连接
        sockaddr_in client_addr{};
        int client_addr_len = sizeof(client_addr);

        SOCKET client_socket = accept(server_socket,
            (sockaddr*)&client_addr,
            &client_addr_len);
        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
            continue;
        }

        // 处理客户端
        client_threads.push_back(std::thread(&Server::handle_client, this, client_socket, client_addr));
        std::cout << "Client connected, total clients: " << client_threads.size() << std::endl;
        if (running.load() == false && client_threads.size() == DEFAULT_PLAYER_NUMBER) {
            startGame();
            running.store(true);
            std::cout << "Game started." << std::endl;
        }
    }
}
void Server::handle_client(SOCKET client_socket, sockaddr_in client_addr) {

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    std::cout << "Client connected: " << client_ip << std::endl;

    // 简单的回显服务
    char buffer[1024];
    int bytes_received;

    // 阻塞直到服务器开始运行（running == true）
    while (!running.load(std::memory_order_acquire)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    while (running.load() && (bytes_received = recv(client_socket, buffer, sizeof(buffer), 0))) {
        std::cout << "Received from " << client_ip << ": "
            << std::string(buffer, bytes_received) << std::endl;
        // 回显数据
        send(client_socket, buffer, bytes_received, 0);
        std::this_thread::sleep_until(start_time + std::chrono::milliseconds(16));
    }

    std::cout << "Client disconnected: " << client_ip << std::endl;
    closesocket(client_socket);
}
void Server::updateGame() {
    while (running.load()) {
        start_time = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_until(start_time + std::chrono::milliseconds(16));
    }
}
void Server::startGame() {
    game_thread = std::thread(&Server::updateGame, this);
}
Server::~Server() {
    if (server_socket != INVALID_SOCKET) {
        closesocket(server_socket);
    }
    for (auto& thread : client_threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    if (game_thread.joinable()) {
        game_thread.join();
    }
    client_threads.clear();
    WSACleanup();
}

int main() {
    Server server;

    if (!server.initialize()) {
        return 1;
    }
    if (!server.start(8888)) {
        return 1;
    }

    server.run();
    return 0;
}