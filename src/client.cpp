#include "client.h"

bool  Client::initialize() {
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

void Client::run() {
    std::string input = "Require map data";
    char buffer[5000];
    while (true) {

        // std::cout << "Enter message (or 'quit' to exit): ";
        // std::getline(std::cin, input);

        if (input == "quit") {
            break;
        }

        // 发送数据
        if (send(client_socket, input.c_str(), input.length(), 0) == SOCKET_ERROR) {
            std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
            break;
        }

        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            recieveData(buffer, bytes_received);
            for (int i = 0;i < DEFAULT_MAP_HEIGHT;i++) {
                for (int j = 0;j < DEFAULT_MAP_WIDTH;j++) {
                    Square& sq = map.getSquare(i, j);
                    std::cout << sq.num << " ";
                }
                std::cout << std::endl;
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
void Client::recieveData(const void* data, int length) {
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

    std::cout << "Map data received. Size: " << header->height << "x" << header->width << std::endl;
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
int main() {
    Client client;

    if (!client.initialize()) {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }

    if (!client.connect_to_server("127.0.0.1", 8888)) {
        return 1;
    }

    client.run();
    return 0;
}