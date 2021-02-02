#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#define BUFFER 11
char buf[BUFFER];
 
int main(int argc, char *argv[]) {
    char bytes[BUFFER], result[BUFFER], ip[256];
    int soc, r, hex;
    struct sockaddr_in adr; 

    sprintf(ip, "%s", "150.254.79.243");
    soc = socket(AF_INET, SOCK_DGRAM, 0);
    adr.sin_family = AF_INET;
    adr.sin_port = htons(4446);
    adr.sin_addr.s_addr = inet_addr(ip);

    sprintf(bytes, "%s", argv[1]);
    hex = atoi(bytes);
    hex = htonl(hex);
    sprintf(result, "%c%c%c%c%c%c\n%d", bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5], hex);    
    memcpy(result+7, (char *)&hex, sizeof(hex));
 
    r = sendto(soc,
       result,
       BUFFER,
       0,
       (struct sockaddr*) &adr,
       sizeof(adr));

    if (r != BUFFER) 
        printf("sendto() nie powiodl sie\n");
    else 
        printf("Wiadomosc wyslana.\n");

    close(soc);
    return 0;
}