#include <stdio.h>
#include <winsock2.h>
#include <process.h>
#pragma comment(lib, "ws2_32.lib")

#define SERV_TCP_PORT 6000
#define BUFFER_SIZE 1024

DWORD WINAPI ClientThread(LPVOID lpParam) {
    SOCKET clientSocket = (SOCKET)lpParam;
    char szBuf[BUFFER_SIZE];
    int nRet;

    nRet = recv(clientSocket, szBuf, BUFFER_SIZE, 0);
    if (nRet > 0) {
        szBuf[nRet] = '\0';
        printf("Received from client: %s\n", szBuf);

        strcpy(szBuf, "From the Server");
        send(clientSocket, szBuf, strlen(szBuf), 0);
    }

    closesocket(clientSocket);
    return 0;
}

int main() {
    WSADATA wsaData;
    SOCKET listenSocket, clientSocket;
    SOCKADDR_IN serverAddr, clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    HANDLE hThread;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        return 1;
    }

    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        printf("Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(SERV_TCP_PORT);
    if (bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Bind failed: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    if (listen(listenSocket, 5) == SOCKET_ERROR) {
        printf("Listen failed: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    printf("TCP Server listening on port %d...\n", SERV_TCP_PORT);

    while (1) {
        clientSocket = accept(listenSocket, (SOCKADDR*)&clientAddr, &clientAddrLen);
        if (clientSocket == INVALID_SOCKET) {
            printf("Accept failed: %d\n", WSAGetLastError());
            break;
        }

        printf("Client connected from [%s:%d]\n", 
               inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

        hThread = CreateThread(NULL, 0, ClientThread, (LPVOID)clientSocket, 0, NULL);
        if (hThread == NULL) {
            printf("CreateThread failed: %d\n", GetLastError());
            closesocket(clientSocket);
        } else {
            CloseHandle(hThread);
        }
    }

    closesocket(listenSocket);
    WSACleanup();
    return 0;
}