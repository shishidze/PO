#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")

int main(){
    WSADATA ws;
    if(WSAStartup(MAKEWORD(2,2), &ws) != 0) {
        printf("Error\n");
        exit(1);
    }

    SOCKET s;
    s = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN sa;
        memset(&sa, 0, sizeof(sa));
        sa.sin_family = AF_INET;
        sa.sin_port = htons(1234);

        bind(s,(const struct sockaddr *) &sa, sizeof(sa));

        listen(s, 100);

        char *buf[200];
        memset(buf,0,sizeof(buf));

        int len;
            SOCKET client_socket;
            SOCKADDR_IN client_addr;
            int client_addr_size = sizeof(client_addr);

        while (client_socket = accept(s,(struct sockaddr *) &client_addr, &client_addr_size)) {

            if(client_socket == 0) printf("Error #2\n");
            else printf("connect OK\n");

            while (recv(client_socket, (char*) buf,sizeof(buf),0) > 0){
                printf("%s\n",buf);
                // send(client_socket,(const char*) 1, sizeof(int), 0);
            }
        }

    closesocket(s);

    return 0;
}

