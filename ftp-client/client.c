#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define PORT 8080
#define IP "192.168.0.110"
#define SA struct sockaddr
#define DOWNLOAD_ARG 2
#define UPLOAD_ARG 1

void upload_to_server(int sockfd, char fileName[50]);
void download_from_server(int sockfd, char fileName[50]);
void connect_to_server(int transferArgument, char fileName[50]);

int main(int argc, char const *argv[])
{
	int transferArgument;
    FILE *arquivo;
    char fileName[30];
    char transferType[30];

    if(argc!=3){
        printf(">> Error! Invalid number of arguments.\n");
        return 0;
    }

    bzero(transferType, 30);
    bzero(fileName, 30);
    strcpy(transferType, argv[1]);
    strcpy(fileName, argv[2]);

    if(strcmp(transferType, "1")==0){
        transferArgument = UPLOAD_ARG;
        arquivo = fopen(fileName, "r");
        if(arquivo==NULL){
            fclose(arquivo);
            return 0;
        }
    }else if(strcmp(transferType, "2")==0){
        transferArgument = DOWNLOAD_ARG;
        printf("%d - ", DOWNLOAD_ARG);
    }else{
        return 0;
    }
    fclose(arquivo);
    connect_to_server(transferArgument, fileName);
    return 0;
}

void upload_to_server(int sockfd, char fileName[50]){
	char buff[BUFFER_SIZE];
    int i;
    char character;
    FILE *arquivo;
    arquivo = fopen(fileName, "r");
    i=0;
    if(arquivo!=NULL){    
        bzero(buff, BUFFER_SIZE);
        strcpy(buff, "upload");
        write(sockfd, buff, sizeof(buff));  
        bzero(buff, BUFFER_SIZE);
        strcpy(buff, fileName);  
        write(sockfd, buff, sizeof(buff));  
        bzero(buff, BUFFER_SIZE);
        do{
            character = fgetc(arquivo);
            if(i>=BUFFER_SIZE){
                printf("%s", buff);
                write(sockfd, buff, sizeof(buff));
                i=0;
                bzero(buff, BUFFER_SIZE);
            }
            buff[i]=character;
            i++;
        }while(character!=EOF);
        printf("%s\n", buff);
        write(sockfd, buff, sizeof(buff));
        i=0;
        bzero(buff, BUFFER_SIZE);
    }	
}

void connect_to_server(int transferArgument, char fileName[50]){
    int sockfd, connfd;
	struct sockaddr_in servaddr, cli;
    // socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf(">> Socket creation failed.\n");
		exit(0);
	}else
		printf(">> Socket successfully created.\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(IP);
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf(">> Connection with the server failed.\n");
		exit(0);
	}else
		printf(">> Connected to the server.\n");

	// upload or download
    if(transferArgument==UPLOAD_ARG){
    	upload_to_server(sockfd, fileName);
    }else{
        download_from_server(sockfd, fileName);
    }
	// close the socket
	close(sockfd);
}

void download_from_server(int sockfd, char fileName[50]){
    char buff[BUFFER_SIZE];
    int i;
    char character;
    FILE *arquivo;
    bzero(buff, BUFFER_SIZE);
    strcpy(buff, "download");
    write(sockfd, buff, sizeof(buff));

    bzero(buff, BUFFER_SIZE);
    strcpy(buff, fileName);
    write(sockfd, buff, sizeof(buff));

    bzero(buff, BUFFER_SIZE);
    read(sockfd, buff, sizeof(buff));
    if(strcmp(buff, "OK")!=0){
        printf(">> Error! The file doesn't exist.\n");
    }else{
        arquivo = fopen(fileName, "w");
        i=0;
        if(arquivo!=NULL){
            while(i<1024){
                bzero(buff, BUFFER_SIZE);
                read(sockfd, buff, sizeof(buff));
                fprintf(arquivo, "%s", buff);
                i++;
            }
            fclose(arquivo);
        }
    }
}
