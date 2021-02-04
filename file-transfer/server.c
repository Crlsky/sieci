#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024

void write_file(int sockfd, char *filename){
  int n = 1;
  FILE *fp;
  char buffer[n];

  fp = fopen(filename, "w");
  while(1) {
    n = recv(sockfd, buffer, sizeof(buffer), 0);

    if (n <= 0 || strcmp(buffer, "\x00") == 0){
      break;
      return;
    }
    
    fwrite(buffer, 1, sizeof(buffer), fp);
  }
  fclose(fp);
  return;
}

void send_file(char *filename, int sockfd){
  int n = 1;
  char data[n];
  FILE *fp;

  fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("[-]Error in reading file.");
    exit(1);
  }

  while(!feof(fp)) {
    fread(data, sizeof(data), 1, fp);

    if (send(sockfd, data, sizeof(data), 0) == -1) {
      perror("[-]Error in sending file.");
      exit(1);
    }
    bzero(data, sizeof(data));
  }
  fclose(fp);
  return;
}

int main(int argc, char *argv[]){
  char *filename = argv[1];
  int port = 8080;
  int e;

  int sockfd, new_sock;
  struct sockaddr_in server_addr, new_addr;
  socklen_t addr_size;
  char buffer[SIZE];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Server socket created successfully.\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = INADDR_ANY;

  e = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(e < 0) {
    perror("[-]Error in bind");
    exit(1);
  }
  printf("[+]Binding successfull.\n");

  if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		perror("[-]Error in listening");
    exit(1);
	}

  addr_size = sizeof(new_addr);
  new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
  write_file(new_sock, filename);

  printf("[+]Data written in the file successfully.\n");

  return 0;
}