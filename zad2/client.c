#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>


char buf[255];

int main(int argc, char **argv)
{
    struct sockaddr_in adr;
    int gniazdo, r, msg = 457958;
    unsigned int port;
    unsigned char bytes[4];
    char index[] = "457958\n";
    char a[3];

    for(int i = 0; i <= sizeof(index)-1; i++){
        sprintf(a, "%02X", index[i]);
        strcat(buf, a);
    }

    printf("Podaj numer portu odbiorcy: ");
    scanf("%u", &port);
    gniazdo = socket(AF_INET, SOCK_DGRAM, 0);
    adr.sin_family = AF_INET;
    adr.sin_port = htons(port);
    adr.sin_addr.s_addr = inet_addr("150.254.77.129");
    
    r = sendto(gniazdo,
               buf,
               255,
               0,
               (struct sockaddr*) &adr,
               sizeof(adr));
    if (r != 255) printf("sendto() nie powiodl sie\n");
    else printf("Wiadomosc wyslana.\n");

    bytes[0] = (msg >> 24) & 0xFF;
    bytes[1] = (msg >> 16) & 0xFF;
    bytes[2] = (msg >> 8) & 0xFF;
    bytes[3] = msg & 0xFF;
    
    sprintf(buf,"%X%X%X%X", bytes[0], bytes[1], bytes[2], bytes[3]);

    r = sendto(gniazdo,
               buf,
               255,
               0,
               (struct sockaddr*) &adr,
               sizeof(adr));
    if (r != 255) printf("sendto() nie powiodl sie\n");
    else printf("Wiadomosc wyslana.\n");

    close(gniazdo);
    return 0;
}