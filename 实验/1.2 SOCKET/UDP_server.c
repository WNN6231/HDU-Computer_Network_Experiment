#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define SERV_UDP_PORT 6000
#define BUFFER_SIZE 1024

#define PRINTERROR(s) fprintf(stderr, "\n%s:%d\n", s, WSAGetLastError())

void DatagramServer(short nPort) {
    SOCKET theSocket;
    SOCKADDR_IN saServer, saClient;
    char szBuf[BUFFER_SIZE];
    int nRet, nLen = sizeof(saClient);

    theSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (theSocket == INVALID_SOCKET) {
        PRINTERROR("socket()");
        return;
    }

    saServer.sin_family = AF_INET;
    saServer.sin_addr.s_addr = htonl(INADDR_ANY);
    saServer.sin_port = htons(nPort);
    if (bind(theSocket, (LPSOCKADDR)&saServer, sizeof(saServer)) == SOCKET_ERROR) {
        PRINTERROR("bind()");
        closesocket(theSocket);
        return;
    }

    printf("UDP Server listening on port %d...\n", nPort);

    while (1) {
        memset(szBuf, 0, BUFFER_SIZE);
        nRet = recvfrom(theSocket, szBuf, BUFFER_SIZE, 0, (LPSOCKADDR)&saClient, &nLen);
        if (nRet > 0) {
            printf("Received from client [%s:%d]: %s\n", 
                   inet_ntoa(saClient.sin_addr), ntohs(saClient.sin_port), szBuf);

            strcpy(szBuf, "From the Server");
            sendto(theSocket, szBuf, strlen(szBuf), 0, (LPSOCKADDR)&saClient, nLen);
        }
    }

    closesocket(theSocket);
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        return 1;
    }

    DatagramServer(SERV_UDP_PORT);
    WSACleanup();
    return 0;
}