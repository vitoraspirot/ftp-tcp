#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define BUFFER_SIZE 30
#define PORT 8080
#define SA struct sockaddr

// Function designed for chat between client and server.
void func(int sockfd)
{
    int i; 
	char fileName[30];
	char transferType[30];
	char buff[BUFFER_SIZE];
	char character;
	FILE *arquivo;
    i=0;
    bzero(buff, BUFFER_SIZE);

    read(sockfd, buff, sizeof(buff));
	strcpy(transferType, buff);
    printf(">> Transfer type: %s\n", transferType);
    bzero(buff, BUFFER_SIZE);

    read(sockfd, buff, sizeof(buff));
    strcpy(fileName, buff);
	bzero(buff, BUFFER_SIZE);
	printf(">> File name: %s\n", fileName);

	if(strcmp(transferType, "upload")==0){
		arquivo = fopen(fileName, "w");
		if(arquivo==NULL){
			printf(">> Error! Não foi possivel criar o arquivo.\n");
		}else{
			while(i<1024){
				bzero(buff, BUFFER_SIZE);
				read(sockfd, buff, sizeof(buff));
				fprintf(arquivo,"%s", buff);
				i++;
			}	
		}
		fclose(arquivo);
	}else{
		printf("<< File Name: %s >>\n", fileName);
		arquivo = fopen(fileName, "r");
		if(arquivo==NULL){
			printf(">> Error! The file from download doesn't exist.\n");
		}else{
			character = fgetc(arquivo);
			while (character!=EOF){
				if(i>=BUFFER_SIZE){
					write(sockfd, buff, sizeof(buff));
					bzero(buff, BUFFER_SIZE);
					i=0;
				}
				buff[i]=character;
				character = fgetc(arquivo);
				i++;
			}
			write(sockfd, buff, sizeof(buff));     
			fclose(arquivo);
		}
	}	
}

// Driver function
int main()
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf(">> Socket creation failed...\n");
		exit(0);
	}
	else
		printf(">> Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf(">> Socket bind failed...\n");
		exit(0);
	}
	else
		printf(">> Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf(">> Listen failed...\n");
		exit(0);
	}
	else
		printf(">> Server listening..\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf(">> Server accept failed...\n");
		exit(0);
	}
	else
		printf(">> Server accept the client...\n");

	// Function for chatting between client and server
	func(connfd);

	// After chatting close the socket
	close(sockfd);
}
