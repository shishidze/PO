#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS

SOCKET Connection;

// typedef char* string;

typedef enum{
    P_ChatMessage,
    P_Test
} Packet;

int ProcessPacket(Packet packettype) {
    switch(packettype) {
        case P_ChatMessage:
        {
            int msg_size;
            recv(Connection, (char *) &msg_size, sizeof(int), 0);
            char *msg = (char*)malloc((msg_size + 1) * sizeof(char));
            msg[msg_size] = '\0';
            recv(Connection, msg, msg_size, 0);
                Packet msgtype = P_ChatMessage;
                send(Connection, (char *) &msgtype, sizeof(Packet), 0);
                send(Connection, (char *) &msg_size, sizeof(int), 0);
                send(Connection, msg, msg_size, 0);
            free(msg);
            break;
        }
        default:
            printf("Unrecognized packet: %d\n", packettype);
            break;
    }

    return 1;
}

void ClientHandler() {
    Packet packettype;
    while(1) {
        recv(Connection, (char *) &packettype, sizeof(Packet), 0);

        if(!ProcessPacket(packettype)) {
            break;
        }
    }
    closesocket(Connection);
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

    SOCKET sListen = socket(AF_INET, SOCK_STREAM, 0);
    bind(sListen, (struct sockaddr*)&addr, sizeof(addr));
    listen(sListen, SOMAXCONN);

    // SOCKET newConnection;
    // // for(int i = 0; i < MAX_CONNECTIONS; i++) {
    //     newConnection = accept(sListen, (struct sockaddr*)&addr, &sizeofaddr);

        if(sListen == 0) {
            printf("Error #2\n");
        } else {
            printf("Client Connected!\n");

            char *msg[200];
            int count = 0;
            for (int i = 0; i < 200; i++){
                char symbol = getchar();
                while (symbol != '\n') {
                    *msg[i] = symbol;
                    count++;
                }
            }
			//int msg_size = sizeof(msg);
			Packet msgtype = P_ChatMessage;
			send(sListen, (char*)&msgtype, sizeof(Packet), 0);
			send(sListen, (char*)&count, sizeof(int), 0);
			send(sListen, (const char *)msg, count, 0);
            Sleep(300);
			// Connections[i] = newConnection;
			// Counter++;
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ClientHandler, 0, 0, 0);

			Packet testpacket = P_Test;
			send(sListen, (char*)&testpacket, sizeof(Packet), 0);
		}
	// }


	system("pause");
	return 0;
}