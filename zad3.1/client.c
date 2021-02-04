#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#define BUFFER_SIZE 30000
#define ATTEMPTS 100

unsigned char buf[BUFFER_SIZE];

int main(int argc, char **argv)
{
    struct sockaddr_in adr;
    int gniazdo, r, i;
    socklen_t addrlen;
    unsigned int port = 6999;
    char abcd[512] = "127.0.0.1";   
    struct timeval time_b, time_e;
 


    if ((gniazdo = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("socket() nie powiodl sie\n");
        return 1;
    }

    adr.sin_family = AF_INET;
    adr.sin_port = htons(port);
    adr.sin_addr.s_addr = inet_addr(abcd);
    addrlen = sizeof(struct sockaddr_in);


    gettimeofday(&time_b, NULL);
    for (i=0; i<ATTEMPTS; i++) {
        r = sendto(gniazdo,
                buf,
                BUFFER_SIZE,
                0,
                (struct sockaddr*) &adr,
                addrlen);

        if (r != BUFFER_SIZE) printf("sendto() nie powiodl sie: %d\n", r);
        else printf("Wiadomosc wyslana.\n");
    
        r = 0;
        while (r < BUFFER_SIZE) {
            r += recvfrom(gniazdo,
                            buf,
                            BUFFER_SIZE-r,
                            0,
                            (struct sockaddr*) &adr,
                            &addrlen);
            
            printf("reciv %d\n", r);
        }
    }
	close(gniazdo);
    gettimeofday(&time_e, NULL);

    printf("czas: %.6f s\n",
        (((double) (time_e.tv_sec - time_b.tv_sec) * 1000000) +
        ((double) (time_e.tv_usec - time_b.tv_usec)))
        / (1000000.0 * ATTEMPTS));
    return 0;
}