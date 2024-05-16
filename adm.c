#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int changePort(int newPort) {
    printf("Do you want to change input port? [y / n] \n");
    char decision;
    while (decision != 'y' && decision != 'Y' && decision != 'n' && decision != 'N') {
        //decision = getchar();
        scanf("%c", &decision);
        switch (decision){
            case 'y' :
                printf("Enter new port number(1024 - 49151):\n");
                scanf("%d",&newPort);
                if (newPort < 1023 || newPort > 49152){
                    while (newPort < 1023 || newPort > 49152){
                        printf("Incorrect input. Try again\n");
                        scanf("%d",&newPort);
                    }
                }
                break;
            case 'Y' :
                printf("Enter new port number(1024 - 49151):\n");
                scanf("%d",&newPort);
                if (newPort < 1023 || newPort > 49152){
                    while (newPort < 1023 || newPort > 49152){
                        printf("Incorrect input. Try again\n");
                        scanf("%d",&newPort);
                    }
                }
                break;
            case 'n' :
                break;
            case 'N' :
                break;
            default :
                printf("Incorrect input. Try again\n");
                // decision = getchar();
                scanf("%c", &decision);
        }
    }
    return newPort;
}

int menu(int depth_coef){
    printf("Enter the decomposition level of a packet:\n 1 - Eth-frame;\n 2 - Ip-packet\n 3 - TCP-packet\n 4 - Data\n");
    while(depth_coef != 1 && depth_coef != 2 && depth_coef != 3 && depth_coef != 4) {
        //if (
        scanf("%d", &depth_coef);
        //!= 1 && (int)depth_coef != depth_coef && getchar() != '\n') printf("Incorrect input. Try again\n");
        // depth_coef = getchar();
        switch (depth_coef){
            case 1:
                printf("Ethernet\n");
                break;
            case 2:
                printf("IP\n");
                break;
            case 3:
                printf("TCP\n");             
                break;
            case 4:
                printf("Data\n");
                break;
            default:
                printf("Incorrect input. Try again\n");
                scanf("%d", &depth_coef);
        }
    }
    return depth_coef;
}

int main(){
    int depth = 0;
    int port = 0;
    depth = menu(depth);
    getchar();
    port = changePort(port);

    int infoconn;
    struct sockaddr_in addr;

    infoconn = socket(AF_INET, SOCK_STREAM, 0);

    if (infoconn < 0){
        perror("uninitialized socket\n");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (connect(infoconn, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("connection failed\n");
        exit(2);
    }

    send(infoconn, &depth, sizeof(depth), 0);
    send(infoconn, &port, sizeof(port), 0);

    char Eth_frame_TCP[10000] = "\0";
    //char Eth_frame_UDP;

    recv(infoconn, Eth_frame_TCP, sizeof(Eth_frame_TCP), 0);
    //recv(infoconn, Eth_frame_UDP, sizeof(Eth_frame_UDP), 0);

    printf("%s\n",Eth_frame_TCP);
    sleep(10);

    close(infoconn);

    return 0;
}
