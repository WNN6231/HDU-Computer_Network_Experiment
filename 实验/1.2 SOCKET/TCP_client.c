#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define SERV_TCP_PORT 6000
#define SERV_HOST_ADDR "127.0.0.1" 
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    SOCKADDR_IN serverAddr;
    char sendBuf[] = "Hello TCP Server!";
    char recvBuf[BUFFER_SIZE];

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        return 1;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        printf("Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERV_TCP_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);

    if (connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Connect failed: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    printf("Connected to server.\n");

    if (send(clientSocket, sendBuf, strlen(sendBuf), 0) == SOCKET_ERROR) {
        printf("Send failed: %d\n", WSAGetLastError());
    } else {
        printf("Sent to server: %s\n", sendBuf);
    }

    int recvResult = recv(clientSocket, recvBuf, BUFFER_SIZE, 0);
    if (recvResult > 0) {
        recvBuf[recvResult] = '\0';
        printf("Server response: %s\n", recvBuf);
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}