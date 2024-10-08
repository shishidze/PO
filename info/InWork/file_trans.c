#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
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

    char c;
    printf("s - server\nk - client\n");
    scanf("%c", &c);

    if (c == 'k') {
        sa.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

        connect(s,(const struct sockaddr *) &sa, sizeof(sa));
    }

    if (c == 's') {
        bind(s,(const struct sockaddr *) &sa, sizeof(sa));

        listen(s, 100);

            SOCKET client_socket;
            SOCKADDR_IN client_addr;
            int client_addr_size = sizeof(client_addr);

        while (client_socket = accept(s,(struct sockaddr *) &client_addr, &client_addr_size)) {
            printf("connect OK\n");
        }
    }

    return 0;
}