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
        char *msg[200];
        char res;
        do {
          printf("message: ");
          scanf("\n%s",msg);
          send(s,(const char*) msg, sizeof(msg), 0);
        } while (1);
    }

    if (c == 's') {
        bind(s,(const struct sockaddr *) &sa, sizeof(sa));

        listen(s, 100);

        char *buf[200];
        memset(buf,0,sizeof(buf));
            SOCKET client_socket;
            SOCKADDR_IN client_addr;
            int client_addr_size = sizeof(client_addr);

        while (client_socket = accept(s,(struct sockaddr *) &client_addr, &client_addr_size)) {
            printf("connect OK\n");

            while (recv(client_socket, (char*) buf,sizeof(buf),0)>0) 
                printf("%s\n",buf);
        }
    }

    closesocket(s);

    return 0;
}
// #include <stdio.h>
// #include <conio.h>
// #include <winsock2.h>
// #pragma comment (lib, "ws2_32.lib")

// #define MAX_PACKET_SIZE    0x10000
// #define SIO_RCVALL   0x98000001
// // Буфер для приёма данных
// static BYTE Buffer[MAX_PACKET_SIZE]; // 64 Kb

// typedef struct _IPHeader
// {
//   unsigned char  verlen;   // версия и длина заголовка
//   unsigned char  tos;      // тип сервиса 
//   unsigned short length;   // длина всего пакета 
//   unsigned short id;       // Id 
//   unsigned short offset;   // флаги и смещения 
//   unsigned char  ttl;      // время жизни 
//   unsigned char  protocol; // протокол 
//   unsigned short xsum;     // контрольная сумма 
//   unsigned long  src;      // IP-адрес отправителя 
//   unsigned long  dest;     // IP-адрес назначения 
// }IPHeader;

// void main()
// {
//   WSADATA     wsadata;   // Инициализация WinSock.
//   SOCKET      s;         // Cлущающий сокет.
//   char        name[128]; // Имя хоста (компьютера).
//   HOSTENT*    phe;       // Информация о хосте.
//   SOCKADDR_IN sa;        // Адрес хоста
//   long        flag = 1;  // Флаг PROMISC Вкл/выкл.

//   // инициализация
//   WSAStartup(MAKEWORD(2,2), &wsadata);
//   s = socket( AF_INET, SOCK_RAW, IPPROTO_IP );
//   gethostname(name, sizeof(name));
//   phe = gethostbyname( name );
//   ZeroMemory( &sa, sizeof(sa) );
//   sa.sin_family = AF_INET;
//   sa.sin_addr.s_addr = ((struct in_addr *)phe->h_addr_list[0])->s_addr;
//   bind(s, (SOCKADDR *)&sa, sizeof(SOCKADDR));
  
//   // Включение promiscuous mode.
//   ioctlsocket(s, SIO_RCVALL, &flag);

//   // Приём IP-пакетов.
//   while( !_kbhit() )
//   {
//     int count;
//     count = recv( s, Buffer, sizeof(Buffer), 0 );
//     // обработка IP-пакета
//     if( count >= sizeof(IPHeader) )
//     {
//       IPHeader* hdr = (IPHeader *) Buffer;
//       //что-то делаем с пакетом...
//       printf("ver=%d tos=%d len=%d id=%d offset=%d ttl=%dms prot=%d xsum=%d src=%s dest=%s", hdr->verlen, hdr->length, hdr->tos, hdr->id, hdr->offset, hdr->ttl, hdr->protocol, hdr->xsum, hdr->src, hdr->dest);
	
//     //        BYTE_H(iph->ver_len),
//     //        BYTE_L(iph->ver_len)*4, 
//     //        iph->tos, 
//     //        ntohs(iph->length),
//     //        ntohs(iph->id), 
//     //        IP_FLAGS(ntohs(iph->flgs_offset)), 
//     //        IP_OFFSET(ntohs(iph->flgs_offset)), 
//     //        iph->ttl, iph->protocol, 
//     //        ntohs(iph->xsum), nethost2str(iph->src),
//     //        nethost2str(iph->dest) 
//     //    );
//     }
//   }
//   // Конец работы.
//   closesocket( s );
// }