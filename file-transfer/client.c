#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 255
#define PORT 6999

void writeFile(int sock){
  int n = 1;
  FILE *fp;
  char buffer[n];
  char filename[SIZE];

  printf("You choose to retrieve file from the server\n");
  printf("What file would you like: ");
  scanf("%s", &filename[0]);

  if (send(sock, filename, strlen(filename), 0) < 0)
    perror("[-]Sent a different number of bytes than expected.\n");

  fp = fopen(filename, "w");
  while(1) {
    n = recv(sock, buffer, sizeof(buffer), 0);

    if (n <= 0 || strcmp(buffer, "\x00") == 0){
      break;
      return;
    }
    fwrite(buffer, 1, sizeof(buffer), fp);
  }
  fclose(fp);
  return;
}

void sendFile(int sock){
  int n = 1;
  char data[n];
  char filename[SIZE];
  FILE *fp;

  printf("You choose to send file to the server\n Which file do you want to send? \n");
  scanf("%s", &filename[0]);

  if (send(sock, filename, strlen(filename), 0) < 0)
    perror("[-] Sent a different number of bytes than expected.\n");

  fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("[-]Error in reading file.");
    exit(1);
  }

  while(!feof(fp)) {
    fread(data, sizeof(data), 1, fp);

    if (send(sock, data, sizeof(data), 0) == -1) {
      perror("[-]Error in sending file.");
      exit(1);
    }
    bzero(data, sizeof(data));
  }
  printf("[+]File data sent successfully.\n");
  fclose(fp);
  return;
}

void fileList(int sock) {
  printf("\nYou choose to recieve a list of files from the server\n\n");
  printf("Below is the returned response from the server:\n");
  /* receive a response from the server */
  bytesRcvd = recv(sock, buffer, SIZE - 1, 0);
  if (bytesRcvd <= 0)
    perror("[-]Revicing failed or connection closed prematurely");

  /* null terminate the received data */
  buffer[bytesRcvd] = '\0';
  printf("%s\n", buffer);    /* Print the echoed arg */
  
  return;
}

void printMenu() {   
    char message[250] = "Select an option from below:\n";
    strcat(message, "(1) List all files on server\n");
    strcat(message, "(2) Retrieve file from server\n");
    strcat(message, "(3) Send file to the server\n");
    strcat(message, "(4) Close Connection\n");
    strcat(message, "Enter your selection: ");
    printf("%s",message);
    return;
}

void butcher(int sock) {
  printf("[x]Closing the connection.\n");
  close(sock);
  printf("[x]Shutting down the client.\n");
  exit(0);
}

int main(int argc, char *argv[]){
  char menuOption[1];
  char *ip = argv[1];
  int e;

  int sock;
  struct sockaddr_in server_addr;

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock < 0) {
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Server socket created successfully.\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = 6999;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  e = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(e == -1) {
    perror("[-]Error in socket");
    exit(1);
  }
	printf("[+]Connected to Server.\n");
  
  while(1){
    printMenu();
    scanf("%s", &menuOption[0]);

    switch(atoi(menuOption)){
      case 1:/* client wants list of all files on server */
        if (send(sock, menuOption, sizeof(menuOption), 0) < 0)
          perror("[-]Sent a different number of bytes than expected");

        fileList(sock);
      case 2:/* client want to retrieve file to server */
        if (send(sock, menuOption, sizeof(menuOption), 0) < 0)
          perror("[-]Sent a different number of bytes than expected.\n");

        writeFile(sock);
        break;
      case 3: /* client want to send file forom server*/
        if (send(sock, menuOption, sizeof(menuOption), 0) < 0)
          perror("[+] Sent a different number of bytes than expected\n");
      
        sendFile(filename, sock);
        break;
      case 4: /* client want to disconnect and exit */
        butcher(sock);
      default:
        printf("You picked an invalid option. Try again.\n");
        break;
    }
  }
  return 0;
}