#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <linux/if.h>
#include <linux/if_packet.h>
#include <linux/udp.h>
#include <net/ethernet.h>
#include <string.h>
#include <error.h>
#include <errno.h>
#include <inttypes.h>

#define REDIRECT_HEADER

#define MAX_PCKT_LENGTH 65535

int main(){
    int sock, listener;
    struct sockaddr_ll addr;
    char buf[1024];
    int bytes_read;

    listener = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW);

    if (listener < 0){
        perror("uninitialized socket\n");
        exit(1);
    }

    // addr.sin_family = AF_INET;
    // addr.sin_port = htons(3425);
    // addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    addr.sll_family = PF_PACKET;
    addr.sll_protocol = htons(ETH_P_IP);

    if ((addr.sll_ifindex = if_nametoindex("ens18")) == 0)
    {
        perror("Interface 'ens18' not found.\n");

        exit(1);
    }

    if (bind(listener, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("connection failed\n");
        exit(2);
    }

    listen(listener,1);

    while (1){
        sock = accept(listener, NULL, NULL);
        if (sock < 0) {
            perror("acception failed\n");
            exit(3);
        }

        while(1) {
            bytes_read = recv(sock, buf, 1024, 0);
            if (bytes_read <= 0 ) break;
            //send (sock, buf, bytes_read, 0);
            for (int i = 0; i < (int)sizeof(buf); i++){
                printf("%c",buf[i]);
            }
        }

        close(sock);
    }

    return 0;
}
