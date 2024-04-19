#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

SOCKET Connection;

typedef enum Packet{
    P_ChatMessage,
    P_Test
} Packet;

int ProcessPacket(Packet packettype) {
    switch(packettype) {
        case P_ChatMessage:
        {
            int msg_size;
            recv(Connection, (char*)&msg_size, sizeof(int), 0);
            char *msg = (char*)malloc((msg_size + 1) * sizeof(char));
            msg[msg_size] = '\0';
            recv(Connection, msg, msg_size, 0);
            printf("%s\n", msg);
            free(msg);
            break;
        }
        case P_Test:
            printf("Test packet.\n");
            break;
        default:
            printf("Unrecognized packet: %d\n", packettype);
            break;
    }

    return 1;
}

void ClientHandler() {
    Packet packettype;
    while(1) {
        recv(Connection, (char*)&packettype, sizeof(Packet), 0);

        if(!ProcessPacket(packettype)) {
            break;
        }
    }
    closesocket(Connection);

    return 0;
}

int main(int argc, char* argv[]) {
    WSADATA wsaData;
    WORD DLLVersion = MAKEWORD(2, 2);
    if(WSAStartup(DLLVersion, &wsaData) != 0) {
        printf("Error\n");
        exit(1);
    }

    SOCKADDR_IN addr;
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(1111);
    addr.sin_family = AF_INET;

    Connection = socket(AF_INET, SOCK_STREAM, 0);
    if(connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
        printf("Error: failed connect to server.\n");
        return 1;
    }
    printf("Connected!\n");

    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ClientHandler, 0, 0, 0);
            send(Connection, (char*)&msgtype, sizeof(Packet), 0);
			send(Connection, (char*)&count, sizeof(int), 0);
			send(Connection, (const char *)msg, count, 0);
            Sleep(300);
    //char msg1[256];
    // while(1) {
    //     fgets(msg1, sizeof(msg1), stdin);
    //     int msg_size = strlen(msg1) - 1;
    //     Packet packettype = P_ChatMessage;
    //     send(Connection, (char*)&packettype, sizeof(Packet), 0);
    //     send(Connection, (char*)&msg_size, sizeof(int), 0);
    //     send(Connection, msg1, msg_size, 0);
    //     Sleep(10);
    // }

    system("pause");
    return 0;
}

