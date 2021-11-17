#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 30
#define MAX_ARRAY_SIZE 30
#define TRANSFER_LIMIT 1024
#define PORT 8080
#define IP "10.0.2.15"
#define SA struct sockaddr
#define DOWNLOAD_ARG 2
#define UPLOAD_ARG 1

void upload_to_server(int sockfd, char fileName[50]);
void download_from_server(int sockfd, char fileName[50]);
void connect_to_server(int transferArgument, char fileName[50]);

/*
 * Validates the input data and connects to the server 
*/
int main(int argc, char const *argv[])
{
	int transferArgument;
    FILE *file;
    char fileName[MAX_ARRAY_SIZE];
    char transferType[MAX_ARRAY_SIZE];

    if(argc!=3){
        printf(">> Error! Invalid number of arguments.\n");
        return 0;
    }

    strcpy(transferType, argv[1]);
    strcpy(fileName, argv[2]);

    if(strcmp(transferType, "upload")==0){
        transferArgument = UPLOAD_ARG;
        file = fopen(fileName, "r");
        if(file==NULL){
            fclose(file);
            return 0;
        }
        fclose(file);
    }else if(strcmp(transferType, "download")==0){
        transferArgument = DOWNLOAD_ARG;
    }else{
        return 0;
    }

    connect_to_server(transferArgument, fileName);
    return 0;
}

/*
 * Connects the client socket to the server socket 
*/
void connect_to_server(int transferArgument, char fileName[MAX_ARRAY_SIZE]){
    int sockfd, connfd;
	struct sockaddr_in servaddr, cli;
    
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf(">> Socket creation failed.\n");
		exit(0);
	}else{
		printf(">> Socket successfully created.\n");
    }
	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(IP);
	servaddr.sin_port = htons(PORT);

	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf(">> Connection with the server failed.\n");
		exit(0);
	}else{
		printf(">> Connected to the server.\n");
    }

    if(transferArgument==UPLOAD_ARG){
    	upload_to_server(sockfd, fileName);
    }else{
        download_from_server(sockfd, fileName);
    }
	
	close(sockfd);
}

/*
 * Uploads a file to the server
*/
void upload_to_server(int sockfd, char fileName[50]){
	char buff[BUFFER_SIZE];
    int i;
    char character;
    FILE *file;
    i=0;
    bzero(buff, BUFFER_SIZE);
    file = fopen(fileName, "r");
    if(file!=NULL){  
        
        strcpy(buff, "upload");
        write(sockfd, buff, sizeof(buff));  
        bzero(buff, BUFFER_SIZE);

        strcpy(buff, fileName);  
        write(sockfd, buff, sizeof(buff));  
        bzero(buff, BUFFER_SIZE);

        character = fgetc(file);
        while (character!=EOF){
            if(i>=BUFFER_SIZE){
                write(sockfd, buff, sizeof(buff));
                bzero(buff, BUFFER_SIZE);
                i=0;
            }
            buff[i]=character;
            character = fgetc(file);
            i++;
        }
        write(sockfd, buff, sizeof(buff));     
        fclose(file);
    }else{
        fclose(file);
        printf(">> Error! The file doesn't exist.\n");
    }	
}

/*
 * Downloads a file from server
*/
void download_from_server(int sockfd, char fileName[50]){
    char buff[BUFFER_SIZE];
    int i;
    char character;
    FILE *file;

    bzero(buff, BUFFER_SIZE);
    strcpy(buff, "download");
    write(sockfd, buff, sizeof(buff));

    bzero(buff, BUFFER_SIZE);
    strcpy(buff, fileName);
    write(sockfd, buff, sizeof(buff));

    file = fopen(fileName, "w");
    if(file==NULL){
        printf(">> Error: Could not create file..\n");
    }else{
        while(i<TRANSFER_LIMIT){
            bzero(buff, BUFFER_SIZE);
            read(sockfd, buff, sizeof(buff));
            fprintf(file,"%s", buff);
            i++;
        }	
    }
    fclose(file);
}


