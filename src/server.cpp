#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER
#define NOKERNEL
#define NOSOUND
#define NOMINMAX

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
    std::cout << "Starting game loop." << std::endl;
    logic_thread = std::thread(&Server::startGame, this);
    //render loop
    while (running.load()) {
        draw();
    }
    if (logic_thread.joinable()) {
        logic_thread.join();
    }
    std::cout << "Game ended." << std::endl;
}
void Server::acceptClientConnections() {
    std::cout << "Waiting for client connections..." << std::endl;
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

        // 处理客户端：把 socket 与对应线程放入同一 ClientInfo 结构中
        auto client = std::make_shared<Server::ClientInfo>();
        client->sock = client_socket;
        client->addr = client_addr;
        client->alive.store(true);
        // 启动线程并在结束时将 alive 标记为 false
        client->id = clients.size() + 1;
        client->th = std::thread(&Server::handle_client, this, client->sock, client->addr, client->id);
        {
            std::lock_guard<std::mutex> lk(client_threads_mutex);
            clients.push_back(client);
            // 清理已结束的客户端（join 并移除）
            cleanup_clients();
        }

        //人数够则开始游戏
        if (running.load() == false && clients.size() == DEFAULT_PLAYER_NUMBER) {
            return;
        }
    }
}
void Server::draw() {
    while (running.load()) {
        // 在这里实现绘图逻辑
        // 例如，发送当前游戏状态给所有客户端
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}
void Server::handle_client(SOCKET client_socket, sockaddr_in client_addr, int client_id) {
    try {
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        std::cout << "Client connected: " << client_ip << std::endl;

        // 简单的回显服务
        char receiveDataBuffer[1024];
        int bytes_received;


        // 阻塞直到服务器开始运行（running == true）
        while (!running.load(std::memory_order_acquire)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }

        while (running.load() && (bytes_received = recv(client_socket, receiveDataBuffer, sizeof(receiveDataBuffer), 0)) >= 0) {
            if (bytes_received == SOCKET_ERROR) {
                int err = WSAGetLastError();
                std::cerr << "Recv failed: " << err << std::endl;
                break;
            }
            if (bytes_received == 0) break;
            handleInput(receiveDataBuffer, bytes_received, client_id);

            int bytes_sent = 0;
            void* sendbuffer = prepareMapdata(bytes_sent);
            send(client_socket, static_cast<char*>(sendbuffer), bytes_sent, 0);
            delete[] static_cast<char*>(sendbuffer);
        }

        clients[client_id - 1]->alive.store(false);
        std::cout << "Client disconnected: " << client_ip << std::endl;
        closesocket(client_socket);
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in handle_client: " << e.what() << std::endl;
        try { closesocket(client_socket); }
        catch (...) {}
    }
    catch (...) {
        std::cerr << "Unknown exception in handle_client" << std::endl;
        try { closesocket(client_socket); }
        catch (...) {}
    }
}
void* Server::prepareMapdata(int& total_size) {
    total_size = sizeof(MapDataHeader) + map.getHeight() * map.getWidth() * sizeof(Square);
    MapDataHeader header;
    header.type = 1; // 假设 1 代表地图数据
    header.height = map.getHeight();
    header.width = map.getWidth();
    header.datasize = map.getHeight() * map.getWidth() * sizeof(Square);
    header.timestamp = std::chrono::high_resolution_clock::now();

    char* buffer = new char[total_size];
    memcpy(buffer, &header, sizeof(MapDataHeader));
    memcpy(buffer + sizeof(MapDataHeader), map.getData(), header.datasize);

    return buffer;
}
void Server::handleInput(const char* data, int length, int client_id) {
    // 处理客户端输入的数据
    ClientInfo& client = *clients[client_id - 1];
    std::string input(data, length);
    std::cout << "Handling input from client " << client_id << ": " << input << std::endl;
    // send(client.sock, data, length, 0);
    // 这里可以根据协议解析数据并更新游戏状态
}
void Server::cleanup_clients() {
    // 必须持有 client_threads_mutex
    std::lock_guard<std::mutex> lk(client_threads_mutex);
    for (auto it = clients.begin(); it != clients.end(); ) {
        auto& client = *it;
        if (!client->alive.load()) {
            if (client->th.joinable()) client->th.join();
            if (client->sock != INVALID_SOCKET) {
                closesocket(client->sock);
                client->sock = INVALID_SOCKET;
            }
            it = clients.erase(it);
        }
        else {
            ++it;
        }
    }
}
void Server::updateGame() {
    // 处理游戏逻辑
    //加一
    for (int i = 0; i < map.getHeight(); ++i) {
        for (int j = 0; j < map.getWidth(); ++j) {
            Square& now = map.getSquare(i, j);
            if (now.id == 0)continue;
            if (now.type == TYPE_LAND && rounds % 50 == 0) {
                now.num += 1; // 土地加一
            }
            if ((now.type == TYPE_CITY || now.type == TYPE_GENERAL) && rounds % 2 == 0) {
                now.num += 1; // 城市和王城加一
            }
        }
    }

    // 处理移动逻辑
    const int dx[] = { 0,0,-1,1 };
    const int dy[] = { -1,1,0,0 };
    for (const auto& move : movements) {
        if (move.dir == NO_ARROW)continue;
        int x = move.x, y = move.y;
        int tox = move.x + dx[move.dir - ARROWUP];
        int toy = move.y + dy[move.dir - ARROWUP];
        Square& now = map.getSquare(x, y);
        Square& to = map.getSquare(tox, toy);
        if (move.id != now.id) {
            continue;
        }
        if (to.type == TYPE_MOUNTAIN) {
            continue;
        }
        if (to.id == now.id) {//同类合并
            to.num += now.num - 1;
            now.num = 1;
            continue;
        }
        if (to.num - (now.num - 1) >= 0) {//异类防守成功
            to.num -= now.num - 1;
            now.num = 1;
        }
        else {//异类防守失败，被攻占
            to.type = now.type;
            to.num = now.num - 1 - to.num;
            to.id = now.id;
            now.num = 1;
        }
    }
    movements.clear();
}

void Server::startGame() {
    start_time = std::chrono::high_resolution_clock::now();
    initMap();
    rounds = 0;
    std::cout << "Game started." << std::endl;
    running.store(true);
    while (running.load()) {
        updateGame();
        if (clients.size() == 0) {
            running.store(false);
            break;
        }
        cleanup_clients();
        std::this_thread::sleep_until(start_time + std::chrono::milliseconds(1000 / speed * (++rounds)));
    }
    running.store(false);
}
void Server::initMap() {
    map.setHeight(DEFAULT_MAP_HEIGHT);
    map.setWidth(DEFAULT_MAP_WIDTH);
    map.init(DEFAULT_PLAYER_NUMBER, DEFAULT_MOUNTAIN_NUMBER, DEFAULT_CITY_NUMBER);
}
void Server::clear() {
    std::cout << "Clearing server state." << std::endl;
    for (auto& client : clients) {
        client->alive.store(false);
        if (client->sock != INVALID_SOCKET) {
            shutdown(client->sock, SD_BOTH);
            closesocket(client->sock);
            client->sock = INVALID_SOCKET;
        }
        if (client->th.joinable()) {
            try { client->th.join(); }
            catch (const std::exception& e) { std::cerr << "Exception joining client thread: " << e.what() << std::endl; }
            catch (...) { std::cerr << "Unknown exception joining client thread" << std::endl; }
        }
    }
    if (logic_thread.joinable()) {
        try { logic_thread.join(); }
        catch (const std::exception& e) { std::cerr << "Exception joining game thread: " << e.what() << std::endl; }
        catch (...) { std::cerr << "Unknown exception joining game thread" << std::endl; }
    }
    std::cout << "Server state cleared." << std::endl;
}
Server::~Server() {
    // signal stop and shutdown server socket to unblock accept/recv
    running.store(false);
    if (server_socket != INVALID_SOCKET) {
        shutdown(server_socket, SD_BOTH);
        closesocket(server_socket);
        server_socket = INVALID_SOCKET;
    }

    for (auto& client : clients) {
        client->alive.store(false);
        if (client->sock != INVALID_SOCKET) {
            shutdown(client->sock, SD_BOTH);
            closesocket(client->sock);
            client->sock = INVALID_SOCKET;
        }
        if (client->th.joinable()) {
            try { client->th.join(); }
            catch (const std::exception& e) { std::cerr << "Exception joining client thread: " << e.what() << std::endl; }
            catch (...) { std::cerr << "Unknown exception joining client thread" << std::endl; }
        }
    }
    if (logic_thread.joinable()) {
        try { logic_thread.join(); }
        catch (const std::exception& e) { std::cerr << "Exception joining game thread: " << e.what() << std::endl; }
        catch (...) { std::cerr << "Unknown exception joining game thread" << std::endl; }
    }

    clients.clear();
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

    while (true) {
        server.acceptClientConnections();
        server.run();
        server.clear();
    }
    return 0;
}