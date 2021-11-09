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
#define DOWNLOAD_ARG 0
#define UPLOAD_ARG 1

void upload_to_server(int sockfd, char fileName[50]);
void download_from_server(int sockfd, char fileName[50]);
void connect_to_server(int transferArgument, char fileName[50]);

int main(int argc, char const *argv[])
{
	int transferArgument;
    FILE *arquivo;
    char fileName[30];
    transferArgument = 0;

    if(argc!=3){
        printf(">> Error! Invalid argument number.\n");
        return 0;
    }
    if(strcmp(argv[1],"upload")!=0&&strcmp(argv[1],"download")!=0){
        printf(">> Error! Invalid transfer argument.\n");
        return 0;
    }
    strcpy(fileName, argv[2]);
    if(strcmp(argv[1],"upload")==0){
        transferArgument=1;
        arquivo = fopen(fileName, "r");
        if(arquivo==NULL){
            printf(">> Error! The file doesn't exist.\n");
            fclose(arquivo);
            return 0;
        }
    }
    fclose(arquivo);
    connect_to_server(transferArgument, fileName);
    return 0;
}

void upload_to_server(int sockfd, char fileName[50]){
	char buff[BUFFER_SIZE];
    int i, rval;
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
        send(sockfd, &buff, sizeof(buff), 0);
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

}
