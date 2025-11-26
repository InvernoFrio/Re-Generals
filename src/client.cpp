#include "client.h"

bool  Client::initialize() {
    running.store(false);
    closed.store(false);
    return WSAStartup(MAKEWORD(2, 2), &wsa_data) == 0;
}

bool Client::connect_to_server(const std::string& host, int port) {
    client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket == INVALID_SOCKET) {
        return false;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    // 解析主机名或IP地址
    if (inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr) <= 0) {
        // 如果直接IP解析失败，尝试DNS解析
        addrinfo hints{}, * result;
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        if (getaddrinfo(host.c_str(), std::to_string(port).c_str(),
            &hints, &result) != 0) {
            std::cerr << "Failed to resolve host: " << host << std::endl;
            return false;
        }

        server_addr = *(sockaddr_in*)result->ai_addr;
        freeaddrinfo(result);
    }

    if (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Connect failed: " << WSAGetLastError() << std::endl;
        return false;
    }

    std::cout << "Connected to " << host << ":" << port << std::endl;
    return true;
}

void Client::handleServerCommunication() {
    std::string input = "Require map data";
    char buffer[10240];
    while (true) {
        if (closed.load())break;
        // 发送请求
        if (running.load() && send(client_socket, input.c_str(), input.length(), 0) == SOCKET_ERROR) {
            std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
            break;
        }

        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            int type = 0;
            if (bytes_received >= static_cast<int>(sizeof(int)))type = *reinterpret_cast<const int*>(buffer);
            if (running.load() == false) {
                if (type == 3) {
                    running.store(true);
                    recieveStartData(static_cast<const void*>(&buffer), bytes_received);
                }
                else {
                    std::cout << "Waiting for game start message..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    return;
                }
            }
            else {
                if (type == 1)recieveMapData(static_cast<const void*>(&buffer), bytes_received);
            }
        }
        else if (bytes_received == 0) {
            std::cout << "Server closed connection" << std::endl;
            break;
        }
        else {
            std::cerr << "Recv failed: " << WSAGetLastError() << std::endl;
            break;
        }
        std::this_thread::sleep_until(last_update_time + std::chrono::milliseconds(1000 / speed));
    }
}
void Client::recieveStartData(const void* data, int length) {
    if (length < sizeof(GameStartMessage)) {
        std::cerr << "Received data too small for GameStartMessage" << std::endl;
        return;
    }

    const GameStartMessage* msg = static_cast<const GameStartMessage*>(data);
    if (msg->type != 3) { // 假设 3 代表游戏开始消息
        std::cerr << "Unknown data type received: " << msg->type << std::endl;
        return;
    }

    id = msg->id;
    general_pos = msg->player_general_pos;
    std::cout << "Received game start message. Client ID: " << id
        << ", General Position: (" << msg->player_general_pos.x
        << ", " << msg->player_general_pos.y << ")" << std::endl;
}
void Client::recieveMapData(const void* data, int length) {
    if (length < sizeof(MapDataHeader)) {
        std::cerr << "Received data too small for MapDataHeader" << std::endl;
        return;
    }

    const MapDataHeader* header = static_cast<const MapDataHeader*>(data);
    if (header->type != 1) { // 假设 1 代表地图数据
        std::cerr << "Unknown data type received: " << header->type << std::endl;
        return;
    }

    last_update_time = header->timestamp;

    int expected_size = sizeof(MapDataHeader) + header->datasize;
    if (length < expected_size) {
        std::cerr << "Received data size mismatch. Expected: " << expected_size << ", Got: " << length << std::endl;
        return;
    }

    map.setHeight(header->height);
    map.setWidth(header->width);
    const Square* squares = reinterpret_cast<const Square*>(
        static_cast<const char*>(data) + sizeof(MapDataHeader));

    // 更新地图数据
    for (int i = 0; i < header->height; i++) {
        for (int j = 0; j < header->width; j++) {
            map.getSquare(i, j) = squares[i * header->width + j];
        }
    }

    std::cout << "Map data received." << std::endl;
}


Client::~Client() {
    if (client_socket != INVALID_SOCKET) {
        closesocket(client_socket);
    }
    try {
        if (network_thread.joinable()) {
            network_thread.join();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception joining network thread: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown exception joining network thread" << std::endl;
    }
    WSACleanup();
}
Map* Client::getMap() {
    return &map;
}
std::deque<Movement>* Client::getMovements() {
    return &movements;
}
int Client::getId() {
    return id;
}
int main() {
    Client client;
    Render render;

    if (!client.initialize()) {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }

    if (!client.connect_to_server("127.0.0.1", 8888)) {
        return 1;
    }

    client.network_thread = std::thread(&Client::handleServerCommunication, &client);
    while (client.running.load() == false) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    render.init(client.getId(), client.getMap(), client.getMovements());
    render.draw();
    client.running.store(false);
    client.closed.store(true);
    client.network_thread.join();
    return 0;
}