#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_PCKT_LENGTH 65535

int changePort(int newPort) {
    printf("Do you want to change input port? [y / n] \n");
    char decision;
    while (decision != 'y' && decision != 'Y' && decision != 'n' && decision != 'N') {
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
                scanf("%c", &decision);
        }
    }
    return newPort;
}

int menu(int depth_coef){
    printf("Enter the decomposition level of a packet:\n 1 - Eth-frame;\n 2 - Ip-packet\n 3 - TCP-packet\n 4 - Data\n");
    while(depth_coef != 1 && depth_coef != 2 && depth_coef != 3 && depth_coef != 4) {
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
    char pckt[MAX_PCKT_LENGTH];
    // char buf[MAX_PCKT_LENGTH];
    int bytes_read;
    int sock;
    depth = menu(depth);
    getchar();
    port = changePort(port);

    int infoconn;
    struct sockaddr_in addr;

    infoconn = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW);

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
    sleep(10);

while (1){
        sock = accept(infoconn, NULL, NULL);
        if (sock < 0) {
            perror("acception failed\n");
            exit(3);
        }

        while(1) {
            bytes_read = recv(sock, pckt, 1024, 0);
            if (bytes_read <= 0 ) break;
            // //send (sock, buf, bytes_read, 0);
            // for (int i = 0; i < (int)sizeof(buf); i++){
            //     printf("%c",buf[i]);
            // }
        }
    }
    printf("Packet info:\n Eth header:\n    dest MAC %02X:%02X:%02X:%02X:%02X:%02X, source MAC %02X:%02X:%02X:%02X:%02X:%02X, Eth hdr len %d, IP type %d\n IP header:\n     IP hdr len %d, IP ver %d, frag offset %d, id 0x%04X, TL proto %d, tos %d, ttl %d, src IP addr %d, dst IP addr %d, tot len %d, checksum 0x%04X\n UDP header:\n     port src %d, dst src %d, len %d, checksum 0x%04X\n", dst.sll_addr[0], dst.sll_addr[1], dst.sll_addr[2], dst.sll_addr[3], dst.sll_addr[4], dst.sll_addr[5], ethhdr->h_source[0], ethhdr->h_source[1], ethhdr->h_source[2], ethhdr->h_source[3], ethhdr->h_source[4], ethhdr->h_source[5], dst.sll_halen, ethhdr->h_proto, iphdr->ihl,  iphdr->version, iphdr->frag_off, iphdr->id, iphdr->protocol, iphdr->tos, iphdr->ttl, iphdr->saddr, iphdr->daddr, iphdr->tot_len, iphdr->check, udphdr->source, udphdr->dest, udphdr->len, udphdr->check);
    close(infoconn);

    return 0;
}
