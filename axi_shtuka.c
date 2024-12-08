#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include "xil_io.h"
#include "xparameters.h"

#define AXI_BASEADDR_BLOCK1 XPAR_AXI_BUS_INTERFACE_0_BASEADDR

#define SOURCEIP_REG 0x00
#define SOURCEPORT_REG 0x08
#define DESTIP_REG 0x0C
#define DESTPORT_REG 0x0C
#define DEPTH_REG 0x14
#define OFFSET8_REG 0x08
#define OFFSET4_REG 0x04

void write_axi_reg(uint32_t base_addr, uint32_t reg_offset, uint32_t value) {
    Xil_Out32(base_addr + reg_offset, value);
}

uint32_t read_from_axi(uint32_t base_addr, uint32_t offset){
    return Xil_In32(base_addr + offset);
}

void read_config_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open config file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *key = strtok(line, "=");
        char *value_str = strtok(NULL, "\n");

        if (key && value_str) {
            uint32_t value = atoi(value_str);
            if (strcmp(key, "SourceIP") == 0) {
                write_axi_reg(SOURCEIP_REG, OFFSET8_REG, inet_addr(value));
            } else if (strcmp(key, "SourcePort") == 0) {
                write_axi_reg(SOURCEPORT_REG, OFFSET4_REG, value);
            } else if (strcmp(key, "DestinationIP") == 0) {
                write_axi_reg(DESTIP_REG, OFFSET8_REG, inet_addr(value));;
            } else if (strcmp(key, "DestinationPort") == 0) {
                write_axi_reg(DESTPORT_REG, OFFSET4_REG, value);
            }
        }
    }
    fclose(file);
}

int main() {

    init_platform();

    int sock, listener, tx_sock;
    struct sockaddr_in addr;
    struct sockaddr_in adm_addr;
    int sent;

    listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (listener < 0){
        perror("closed socket\n");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

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
        if (lineNum == 4){
            if (port != 0) {
                fprintf(tempFile,"DestinationPort=%d\n",port);
                }else {
                    fputs(line,tempFile);
                }
        } else  {
            fputs(line,tempFile);
        }
    }
    
    fclose(tempFile);
    fclose(configFile);

    remove("cfg.txt");
    rename("temp.txt","cfg.txt");

    read_config_file("cfg.txt");

    int send_sock = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW);

    if ((sent = sendto(sockfd, pckt, len, 0, (struct sockaddr *)&dst, sizeof(dst))) < 0){
        if ((sent = write(sockfd, pckt, len)) < 0)
        {
            perror("sendto");
        }
    }

    return 0;
}