#pragma once

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <functional>

#pragma comment(lib, "ws2_32.lib")

class UdpServer {
public:
    using CallbackFunc = std::function<void(const std::string&)>;

    UdpServer(int port, CallbackFunc callback)
        : m_port(port), m_callback(callback), m_serverSocket(INVALID_SOCKET), m_hThread(NULL), m_isRunning(false) {
    }

    bool start() {
        if (!initializeWinsock()) {
            return false;
        }

        if (!createAndBindSocket()) {
            return false;
        }

        m_isRunning = true;
        m_hThread = CreateThread(NULL, 0, StaticThreadStart, this, 0, NULL);
        if (m_hThread == NULL) {
            std::cerr << "CreateThread failed with error: " << GetLastError() << std::endl;
            closesocket(m_serverSocket);
            WSACleanup();
            return false;
        }

        std::cout << "UDP Echo Server is running" << std::endl;
        return true;
    }

    void stop() {
        if (m_isRunning) {
            m_isRunning = false;
            if (m_hThread != NULL) {
                WaitForSingleObject(m_hThread, INFINITE);
                CloseHandle(m_hThread);
                m_hThread = NULL;
            }
        }

        if (m_serverSocket != INVALID_SOCKET) {
            closesocket(m_serverSocket);
            m_serverSocket = INVALID_SOCKET;
        }
        WSACleanup();
        std::cout << "UDP Server stopped." << std::endl;
    }

    ~UdpServer() {
        stop();
    }

private:
    static DWORD WINAPI StaticThreadStart(LPVOID lpParam) {
        UdpServer* server = static_cast<UdpServer*>(lpParam);
        server->run();
        return 0;
    }

    bool initializeWinsock() {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            std::cerr << "WSAStartup failed with error: " << result << std::endl;
            return false;
        }
        return true;
    }

    bool createAndBindSocket() {
        m_serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (m_serverSocket == INVALID_SOCKET) {
            std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return false;
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(m_port);

        int result = bind(m_serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
        if (result == SOCKET_ERROR) {
            std::cerr << "bind failed with error: " << WSAGetLastError() << std::endl;
            closesocket(m_serverSocket);
            WSACleanup();
            return false;
        }

        return true;
    }

    void run() {
        char recvBuf[512];
        sockaddr_in clientAddr;
        int clientAddrLen = sizeof(clientAddr);

        while (m_isRunning) {
            int recvLen = recvfrom(m_serverSocket, recvBuf, sizeof(recvBuf) - 1, 0, (sockaddr*)&clientAddr, &clientAddrLen);
            if (recvLen == SOCKET_ERROR) {
                if (m_isRunning) {
                    std::cerr << "recvfrom failed with error: " << WSAGetLastError() << std::endl;
                }
                continue;
            }

            recvBuf[recvLen] = '\0';

            char clientIP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);

            m_callback(std::string(recvBuf));
        }
    }

private:
    int m_port;
    SOCKET m_serverSocket;
    HANDLE m_hThread;
    std::atomic<bool> m_isRunning;
    CallbackFunc m_callback;
};