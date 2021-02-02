#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h> 
#include <unistd.h> 
#define BUFFER_SIZE 30000

char buf[BUFFER_SIZE];

int main(int argc, char **argv)
{
    struct sockaddr_in myaddr, endpoint;
    int sdsocket, r;
    socklen_t addrlen;
    unsigned int port = 6999;

    if ((sdsocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("socket() nie powiodl sie\n");
        return 1;
    }
        
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(port);
    myaddr.sin_addr.s_addr = INADDR_ANY;

    if (
        bind(sdsocket,
             (struct sockaddr*) &myaddr,
             sizeof(struct sockaddr_in)) < 0) {
        printf("bind() nie powiodl sie\n");
        return 1;
    }

    addrlen = sizeof(struct sockaddr_in);

    /* nieskonczona petla */
    while(1){
        r=0;
        while(r < BUFFER_SIZE){
            r += recvfrom(sdsocket,
                            buf,
                            BUFFER_SIZE-r,
                            0,
                            (struct sockaddr*) &endpoint,
                            &addrlen);

                    printf("reciv %d", r);

        }
        printf("Wiadomosc od %s: %d\n",
                inet_ntoa(endpoint.sin_addr),
                sizeof(buf));

        r = sendto(sdsocket,
                buf,
                BUFFER_SIZE,
                0,
                (struct sockaddr*) &endpoint,
                addrlen);

        if (r != BUFFER_SIZE) printf("sendto() nie powiodl sie\n");
        else printf("Wiadomosc odeslana.\n");
    }
    return 0;
}