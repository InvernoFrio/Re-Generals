#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>


class Client {
private:
    SOCKET client_socket;
    WSADATA wsa_data;

public:
    Client() : client_socket(INVALID_SOCKET) {}

    bool initialize() {
        return WSAStartup(MAKEWORD(2, 2), &wsa_data) == 0;
    }

    bool connect_to_server(const std::string& host, int port) {
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

    void run() {
        std::string input;
        char buffer[1024];

        while (true) {
            std::cout << "Enter message (or 'quit' to exit): ";
            std::getline(std::cin, input);

            if (input == "quit") {
                break;
            }

            // 发送数据
            if (send(client_socket, input.c_str(), input.length(), 0) == SOCKET_ERROR) {
                std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
                break;
            }

            // 接收回显
            int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            if (bytes_received > 0) {
                buffer[bytes_received] = '\0';
                std::cout << "Server echo: " << buffer << std::endl;
            }
            else if (bytes_received == 0) {
                std::cout << "Server closed connection" << std::endl;
                break;
            }
            else {
                std::cerr << "Recv failed: " << WSAGetLastError() << std::endl;
                break;
            }
        }
    }

    ~Client() {
        if (client_socket != INVALID_SOCKET) {
            closesocket(client_socket);
        }
        WSACleanup();
    }
};

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