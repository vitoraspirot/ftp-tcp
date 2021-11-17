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
#define MAX_ARRAY_SIZE 30
#define TRANSFER_LIMIT 1024


/*
 * Downloads and uploads
*/
void transfer(int sockfd)
{
    int i; 
	char fileName[MAX_ARRAY_SIZE];
	char transferType[MAX_ARRAY_SIZE];
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
			while(i<TRANSFER_LIMIT){
				bzero(buff, BUFFER_SIZE);
				read(sockfd, buff, sizeof(buff));
				fprintf(arquivo,"%s", buff);
				i++;
			}	
		}
		fclose(arquivo);
	}else{
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

/*
 * Creates the server and accepts client connections
*/
int main()
{
	int sockfd, socket_client, len;
	struct sockaddr_in servaddr, cli;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf(">> Socket creation failed...\n");
		exit(0);
	}
	else{
		printf(">> Socket successfully created..\n");
	}
	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf(">> Socket bind failed...\n");
		exit(0);
	}
	else{
		printf(">> Socket successfully binded..\n");
	}
	if ((listen(sockfd, 5)) != 0) {
		printf(">> Listen failed...\n");
		exit(0);
	}
	else{
		printf(">> Server listening..\n");
	}
	len = sizeof(cli);

	socket_client = accept(sockfd, (SA*)&cli, &len);
	if (socket_client < 0) {
		printf(">> Server accept failed...\n");
		exit(0);
	}
	else{
		printf(">> Server accept the client...\n");
	}
	transfer(socket_client);
	close(sockfd);
}
