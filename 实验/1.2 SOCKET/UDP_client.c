#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define SERV_UDP_PORT 6000
#define SERV_HOST_ADDR "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    SOCKADDR_IN serverAddr;
    char sendBuf[] = "Hello UDP Server!";
    char recvBuf[BUFFER_SIZE];
    int serverAddrLen = sizeof(serverAddr);

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        return 1;
    }

    clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (clientSocket == INVALID_SOCKET) {
        printf("Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERV_UDP_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);

    if (sendto(clientSocket, sendBuf, strlen(sendBuf), 0, 
              (SOCKADDR*)&serverAddr, serverAddrLen) == SOCKET_ERROR) {
        printf("Send failed: %d\n", WSAGetLastError());
    } else {
        printf("Sent to server: %s\n", sendBuf);
    }

    int recvResult = recvfrom(clientSocket, recvBuf, BUFFER_SIZE, 0, 
                             (SOCKADDR*)&serverAddr, &serverAddrLen);
    if (recvResult > 0) {
        recvBuf[recvResult] = '\0';
        printf("Server response: %s\n", recvBuf);
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}