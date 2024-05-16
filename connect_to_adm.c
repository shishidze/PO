#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 256
// int pack(){


// char   IP_ref = "50 ff 20 9f 96 2d 00 45 e2 04 70 87 08 00 45 00 00 29 95 cc 40 00 80 06 de e2 0a 00 01 10 6c b1 0e 5f fe 17 01 bb 2e d3 a6 f9 64 0e f3 68 50 10 01 ff fa 9d 00 00 00"; //check IP
// char ICMP_ref = "50 ff 20 9f 96 2d 00 45 e2 04 70 87 08 00 45 00 00 3c 3b b6 00 00 80 01 e3 eb 0a 00 01 10 08 08 08 08 08 00 4d 5a 00 01 00 01 61 62 63 64 65 66 67 68 69 6a 6b 6c 6d 6e 6f 70 71 72 73 74 75 76 77 61 62 63 64 65 66 67 68 69"; // ICMP
// char  TCP_ref = "00 45 e2 04 70 87 00 08 e3 ff fc 04 08 00 45 00 05 0a 18 fa 40 00 3a 06 65 46 b9 e2 34 56 0a 20 c5 55 01 bb f7 f1 a0 b8 ab 38 c2 f5 1d 48 50 10 7e 93 85 60 00 00 aa 27 64 84 55 51 d1 71 83 57"; //TCP
// char  UDP_ref = "00 45 e2 04 70 87 00 08 e3 ff fc 04 08 00 45 00 01 58 9a c4 00 00 38 11 7f 65 0a 20 8d 88 0a 20 c5 a3 00 35 c1 80 01 44 7f b8 b4 ee 81 80 00 01 00 01 00 04 00 08 0c 73 61 66 65 62 72 6f 77 73"; //UDP
// }

int main(){
    int sock, listener;
    struct sockaddr_in addr;
    struct sockaddr_in adm_addr;

    listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (listener < 0){
        perror("closed socket\n");
        exit(1);
    }

    FILE *configFile = fopen("config.txt","r");
    if (configFile == NULL) {
        perror("file is not exist/n");
        exit(2);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    //INADDR_ANY;
    //inet_addr("190.160.43.40");
    //

    if (bind(listener, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("connection failed\n");
        exit(3);
    }

    listen (listener,1);
    
    unsigned int addrlen = sizeof(adm_addr);
    int buf;
    int port;

    sock = accept(listener, (struct sockaddr *) &adm_addr, &addrlen);
    if (sock < 0) {
        perror ("accept");
        exit(4);
    }

    recv(sock, &buf, sizeof(buf),0);
    recv(sock, &port, sizeof(port), 0);

    FILE *tempFile = fopen("temp.txt","w+");
    if (tempFile == NULL) {
        perror("temp file is not exist/n");
        exit(5);
    }

    int lineNum = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), configFile)) {
        lineNum++;
        if (lineNum == 8){
            if (port != 0) {
                fprintf(tempFile,"%d\n",port);
                }else {
                    fputs(line,tempFile);
                }
        } else if (lineNum == 11){
            fprintf(tempFile,"%d",buf);
        } else  {
            fputs(line,tempFile);
        }
    }
    
    fclose(tempFile);
    fclose(configFile);

    remove("config.txt");
    rename("temp.txt","config.txt");


    char *Eth_frame_TCP = "00 45 e2 04 70 87 00 08 e3 ff fc 04 08 00 45 00 05 0a 18 fa 40 00 3a 06 65 46 b9 e2 34 56 0a 20 c5 55 01 bb f7 f1 a0 b8 ab 38 c2 f5 1d 48 50 10 7e 93 85 60 00 00 aa 27 64 84 55 51 d1 71 83 57"; // TCP
    //char *Eth_frame_UDP = "00 45 e2 04 70 87 00 08 e3 ff fc 04 08 00 45 00 01 58 9a c4 00 00 38 11 7f 65 0a 20 8d 88 0a 20 c5 a3 00 35 c1 80 01 44 7f b8 80 00 01 00 01 00 04 00 08 0c 73 61 66 65 62 72 6f 77 73"; //UDP

    send(sock, Eth_frame_TCP, sizeof(Eth_frame_TCP), 0);
    //send(sock, Eth_frame_UDP, sizeof(Eth_frame_UDP), 0);


    printf("%d\n", buf);
    printf("%d\n",port);
    printf("gratz\n");
    
    close(sock);

    return 0;
}
