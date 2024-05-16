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


int main()
{
    int sockfd;
    struct sockaddr_ll dst;
    char *pckt[MAX_PCKT_LENGTH] = "this is  packet\n";

    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

    if (sockfd <= 0)
    {
        perror("socket");

        exit(1);
    }

    dst.sll_family = PF_PACKET;
    dst.sll_protocol = htons(ETH_P_IP);

    if ((dst.sll_ifindex = if_nametoindex("ens18")) == 0)
    {
        perror("Interface 'ens18' not found.\n");

        exit(1);
    }

    // Do destination ethernet MAC (08:00:27:54:4c:a1).
    dst.sll_addr[0] = 0x08;
    dst.sll_addr[1] = 0x00;
    dst.sll_addr[2] = 0x27;
    dst.sll_addr[3] = 0x54;
    dst.sll_addr[4] = 0x4C;
    dst.sll_addr[5] = 0xA1;
    dst.sll_halen = ETH_ALEN;

    // I tried doing this with and without bind. Still not working.
    if (bind(sockfd, (struct sockaddr *)&dst, sizeof(dst)) < 0)
    {
        perror("connect");

        exit(1);
    }

    struct ethhdr *ethhdr = (struct ethhdr *) (pckt);
    struct iphdr *iphdr = (struct iphdr *) (pckt + sizeof(struct ethhdr));
    struct udphdr *udphdr = (struct udphdr *) (pckt + sizeof(struct ethhdr) + sizeof(struct iphdr));
    unsigned char *data = (unsigned char *) (pckt + sizeof(struct ethhdr) + sizeof(struct iphdr));

    // Do source ethernet MAC (08:00:27:ad:31:35).
    ethhdr->h_source[0] = 0x08;
    ethhdr->h_source[1] = 0x00;
    ethhdr->h_source[2] = 0x27;
    ethhdr->h_source[3] = 0xAD;
    ethhdr->h_source[4] = 0x31;
    ethhdr->h_source[5] = 0x35;

    for (int i = 0; i < 30; i++)
    {
        memcpy(data + i, "b", 1);
    }

 // Copy destination MAC to sockaddr_ll.
    memcpy(ethhdr->h_dest, dst.sll_addr, ETH_ALEN);

    // Protocol.
    ethhdr->h_proto = htons(ETH_P_IP);

    // Fill out ip header.
    iphdr->ihl = 5;
    iphdr->version = 4;
    iphdr->frag_off = 0;
    iphdr->id = htons(0);
    iphdr->protocol = IPPROTO_UDP;
    iphdr->tos = 0x0;
    iphdr->ttl = 64;
    iphdr->saddr = inet_addr("127.0.0.1");
    iphdr->daddr = inet_addr("127.0.0.1");
    iphdr->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + 30);
    iphdr->check = 0;
    //iphdr->check = ip_fast_csum(iphdr, iphdr->ihl);

    // Fill out UDP header.
    udphdr->source = htons(27000);
    udphdr->dest = htons(27015);
    udphdr->len = htons(sizeof(struct udphdr) + 30);
    udphdr->check = 0;
    //udphdr->check = csum_tcpudp_magic(iphdr->saddr, iphdr->daddr, sizeof(struct udphdr) + 30, IPPROTO_UDP, csum_partial(udphdr, sizeof(struct udphdr) + 30, 0));

// Send packet
    uint16_t sent;
    int len = ntohs(iphdr->tot_len) + sizeof(struct ethhdr) + 30;

    if ((sent = sendto(sockfd, pckt, len, 0, (struct sockaddr *)&dst, sizeof(dst))) < 0)
    //if ((sent = write(sockfd, pckt, len)) < 0)
    {
        perror("sendto");
    }

    fprintf(stdout, "Sent %d of data. %d is IPHdr len. %d is len.\n", sent, iphdr->tot_len, len);

    close(sockfd);

    exit(0);
}

// val dataString1 = "50 ff 20 9f 96 2d 00 45 e2 04 70 87 08 00 45 00 00 29 95 cc 40 00 80 06 de e2 0a 00 01 10 6c b1 0e 5f fe 17 01 bb 2e d3 a6 f9 64 0e f3 68 50 10 01 ff fa 9d 00 00 00" //check IP
// val dataString4 = "00 45 e2 04 70 87 00 08 e3 ff fc 04 08 00 45 00 01 58 9a c4 00 00 38 11 7f 65 0a 20 8d 88 0a 20 c5 a3 00 35 c1 80 01 44 7f b8 b4 ee 81 80 00 01 00 01 00 04 00 08 0c 73 61 66 65 62 72 6f 77 73" //UDP
// val dataString2 = "50 ff 20 9f 96 2d 00 45 e2 04 70 87 08 00 45 00 00 3c 3b b6 00 00 80 01 e3 eb 0a 00 01 10 08 08 08 08 08 00 4d 5a 00 01 00 01 61 62 63 64 65 66 67 68 69 6a 6b 6c 6d 6e 6f 70 71 72 73 74 75 76 77 61 62 63 64 65 66 67 68 69" // ICMP
// val dataString3 = "00 45 e2 04 70 87 00 08 e3 ff fc 04 08 00 45 00 05 0a 18 fa 40 00 3a 06 65 46 b9 e2 34 56 0a 20 c5 55 01 bb f7 f1 a0 b8 ab 38 c2 f5 1d 48 50 10 7e 93 85 60 00 00 aa 27 64 84 55 51 d1 71 83 57" //TCP