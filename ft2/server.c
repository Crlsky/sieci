/*code for server portion of server-client setup*/
/*communication occurs overs TCP*/

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <errno.h>  /* for error numbers */
#include <dirent.h> /* for directories */

#define MAXPENDING 5    /* Maximum outstanding connection requests */
#define MAXLEN 255      /* Longest string  */

int main(int argc, char *argv[])
{
    int servSock;                       /* Socket descriptor for server */
    int clntSock;                       /* Socket descriptor for client */
    struct sockaddr_in echoServAddr;    /* Local address */
    struct sockaddr_in echoClntAddr;    /* Client address */
    unsigned short echoServPort;        /* Server port */
    unsigned int clientOption, clntLen; /* Length of client address data structure */
    char data_recv[MAXLEN+1];       /* data received from client */
    int recvMsgSize;                /* size of message received */
    char data_sent[2000];       /* data sent to client */
    char file_request[50];      /* file requested by client */
    char file[2000];            /* last file sent to client */
    socklen_t len;  


    if (argc != 2)     /* Test for correct number of arguments */
    {
        fprintf(stderr, "Usage:  %s <Server Port>\n", argv[0]);
        exit(1);
    }

    echoServPort = atoi(argv[1]);  /* First arg:  local port */

    /* Create socket for incoming connections */
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        perror("socket() failed");

    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(echoServPort);      /* Local port */

    /* Bind to the local address */
    if (bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        perror("bind() failed");

    /* Mark the socket so it will listen for incoming connections */
    if (listen(servSock, MAXPENDING) < 0)
        perror("listen() failed");

    /* Wait for a client to connect */
    clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen);
     if (clntSock < 0)
            perror("accept() failed");

    /* Run Forever */
    while(atoi(data_recv) != -1){            
    /* Block until receive message from a client */
    recvMsgSize = recv(clntSock, data_recv, sizeof(data_recv), 0);
    if(recvMsgSize < 0)
    {
        printf("Could not block socket. Failed with : %s\n", strerror(errno));
    }
    printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));       
    printf("%d\n", atoi(data_recv));
    recvMsgSize = 1000;
    printf("%d\n", atoi(data_recv));
    char temp[255];

    switch(atoi(data_recv)){
        printf("%d\n", atoi(data_recv));
            case 1:
            printf("Client has requested a list of all files\n");
                DIR  * d;
                struct dirent * dp;
                d = opendir(".");               



                if(d != NULL){
                    while((dp = readdir(d)) != NULL){
                        if(dp->d_type == DT_REG){
                            strcat(temp, dp->d_name);
                            strcat(temp, "\n");
                        }
                    }
                    strcpy(data_recv, temp);
                    strcpy(temp, "");

                        if(send(clntSock, data_recv, recvMsgSize, 0) < 0){
                            printf("send() sent a different number of bytes than expected");
                        }


                    closedir(d);
                }
                break;
            case 2:
                printf("Client has requested a single file. Waiting for file name.\n");
                    /* block until receive message from a client */
                recvMsgSize = recv(clntSock, file_request, sizeof(file_request), 0);
                if(recvMsgSize < 0)
                {
                    printf("Could not block socket\n");
                    printf("Failed with the following error: %s\n", strerror(errno));
                }

                printf("Client has request: %s\n", file_request);

                FILE *fp;
                fp = fopen(file_request, "r");
                if(fp == NULL)
                {
                    // send message to client that file does not exist
                    printf("Specified file does not exist.");
                }

                fseek(fp, 0, SEEK_END);
                size_t file_size = ftell(fp);
                fseek(fp, 0, SEEK_SET);

                    if(fread(file, file_size, 1, fp) <= 0)
                {
                    // send message that unable to copy file to buffer
                    printf("Unable to copy file to buffer");
                    exit(1);
                    }

                if (send(clntSock, file, sizeof(file), 0) < 0)
                    {
                    printf("error in sending file");
                }

                bzero(file, sizeof(file));
                break;
            case 3:
                break;
            case 4:
                printf("Client has closed connection. Waiting on new clients");
                break;
        }
        recvMsgSize = 0;
        data_recv[0] = 0;
    }

    close(servSock);
}