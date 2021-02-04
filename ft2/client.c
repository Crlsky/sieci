/*code for client portion of server-client setup*/
/*communication occurs overs TCP*/

#include <stdio.h>              /* for printf() and fprintf() */
#include <sys/socket.h>     /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>      /* for sockaddr_in and inet_addr() */
#include <stdlib.h>             /* for atoi() and exit() */
#include <string.h>             /* for memset() */
#include <unistd.h>             /* for close() */
#include <stdbool.h>        /* for boolean values */

#define RCVBUFSIZE 255   /* Size of receive buffer */

char * printMenu();

int main(int argc, char *argv[]) {
    int sock,i,num;                  /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    struct sockaddr_in fromAddr;     /* address of data sender */
    unsigned short echoServPort;     /* Echo server port */
    char *servIP;                    /* Server IP address (dotted quad) */
    char *echoString;                /* String to send to echo server */
    char echoBuffer[RCVBUFSIZE];     /* Buffer for echo string */
    unsigned int echoStringLen;      /* Length of string to echo */
    int bytesRcvd, totalBytesRcvd;   /* Bytes read in single recv() 
                                        and total bytes read */
    char menuOption[1];              /* User Menu seclection to send to server */
    char buffer[RCVBUFSIZE+1];       /* Buffer for receiving data from server */
    int recvDataLen;                 /* Length of received response */
    char * menuMsg;          /* used to print menu options */
    char * temp[255] = {0};
    char file_buffer[2000];      /* file sent by server */
    char file_request[50];       /* file requested by client */


    if (argc != 3) {    /* Test for correct number of arguments */
        fprintf(stderr,"Usage: %s <Server IP> [<Port Id>]\n", argv[0]);
        exit(1);
    }

    servIP = argv[1];           /* First arg: server IP address (dotted quad) */

    if (argc == 3) 
        echoServPort = atoi(argv[2]);  /* Use given port, if any */
    else
        fprintf(stderr,"Usage: %s requires [<Port Id>]\n", argv[0]);

    /* Create a reliable, stream socket using TCP */
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0)
        perror("socket() failed");

    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));     /* Zero out structure */
    echoServAddr.sin_family      = AF_INET;             /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    echoServAddr.sin_port        = htons(echoServPort); /* Server port */

    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        perror("connect() failed");

    //echoStringLen = strlen(echoString);          /* Determine input length */

    menuMsg = printMenu();

    while(atoi(menuOption) != -1){
    printf("%s", menuMsg);
    scanf("%s", &menuOption[0]);

    switch(atoi(menuOption)){
            case 1:
                /* send menu choice to server indicating client wants list of all files on server */
                if (send(sock, menuOption, sizeof(menuOption), 0) < 0){
                    printf("sendto() sent a different number of bytes than expected");
                }

                printf("\nYou choose to recieve a list of files from the server\n\n");
                printf("Below is the returned response from the server:\n");

                /* receive a response from the server */
                bytesRcvd = recv(sock, buffer, RCVBUFSIZE - 1, 0);
                if (bytesRcvd <= 0)
                            perror("recv() failed or connection closed prematurely");


                /* null terminate the received data */
                buffer[bytesRcvd] = '\0';
                printf("%s\n", buffer);    /* Print the echoed arg */

                break;
            case 2:
                /* send menu choice to server indicating client wants list of all files on server */
                if (send(sock, menuOption, sizeof(menuOption), 0) < 0){
                    printf("sendto() sent a different number of bytes than expected");
                }

                printf("You choose to retrieve a single file from the server\n");
                printf("What file would you like: ");
                scanf("%s", &file_request[0]);

                if (send(sock, file_request, strlen(file_request), 0) < 0)
                {
                        printf("sendto() sent a different number of bytes than expected");
                }

                /* Recv a response */
                bytesRcvd = sizeof(fromAddr);
                recvDataLen = recv(sock, buffer, sizeof(buffer), 0);

                if (recvDataLen < 0)
                    printf("recvfrom() failed\n");

                char new_file[] = "copied";
                strcat(new_file, buffer);
                FILE *fp;
                fp = fopen(new_file, "w+");

                if(fwrite(buffer, 1, sizeof(buffer), fp) < 0) {
                    printf("error writting file\n");
                    exit(1);
                }
                break;

            case 3:
                    printf("You choose to retrieve all files from the server\n");
                break;
            case 4:
                /* send menu choice to server indicating client wants list of all files on server */
                if (send(sock, menuOption, sizeof(menuOption), 0) < 0){
                    printf("sendto() sent a different number of bytes than expected");
                }

                printf("Closing connection\n");
                close(sock);
                exit(0);
            default:
                printf("You picked an invalid option. Try again.\n");
                break;
        }

    }

    printf("\n");    /* Print a final linefeed */

    close(sock);
    exit(0);
}


char * printMenu()
{   
    static char message[250] = "Select an option from below:\n";
        strcat(message, "(1) List all files on server\n");
    strcat(message, "(2) Retrieve file from server\n");
    strcat(message, "(3) Retrieve all files from server\n");
    strcat(message, "(4) Close Connection\n");
    strcat(message, "Enter your selection: ");
    return message;
}