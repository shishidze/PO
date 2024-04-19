#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment (lib, "ws2_32.lib")

int main(){
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);

    SOCKET s;
    s = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN sa;
        memset(&sa, 0, sizeof(sa));
        sa.sin_family = AF_INET;
        sa.sin_port = htons(1234);

        sa.sin_addr.S_un.S_addr = inet_addr("192.168.1.1");

        connect(s,(const struct sockaddr *) &sa, sizeof(sa));
        char *msg[200];
        do {
          printf("message: ");
          scanf("\n%s200[0-9a-zA-Z.,! ]",msg);
          send(s,(const char*) msg, sizeof(msg), 0);

        } while (1);
            
    closesocket(s);

    return 0;
}
