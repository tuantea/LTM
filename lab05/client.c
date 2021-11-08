#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFF_SIZE 255
int main(int argc, char *argv[])
{
	int sockfd, rcvBytes, sendBytes;
	char buff[BUFF_SIZE], user[20];
	struct sockaddr_in servaddr;
	char *serv_IP;
	short serv_PORT;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s <ServerIP> [<EchoPort>]\n", argv[0]);
		exit(1);
	}
	serv_IP = argv[1];
	serv_PORT = atoi(argv[2]);

//Step 1: Construct socket
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Error: ");
		return 0;
	}
//Step 2: Define the address of the server
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(serv_IP);
	servaddr.sin_port = htons(serv_PORT);
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
    {printf("connected to the server..\n");}
	strcpy(buff, "Hello");
	send(sockfd, buff, strlen(buff), 0);
	recv(sockfd, user, BUFF_SIZE, 0);
//Step 3: Communicate with server
	user[strlen(user)-1] = '\0';
	printf("%s\n", user);
	printf("%d\n",strcmp(user,"user1"));
	while(strcmp(user,"user1") == 0){
			printf("Send to server: ");
			fgets(buff, BUFF_SIZE, stdin);
			if(buff[0] == '\n'){
				strcpy(buff,"exit");
				send(sockfd, buff, strlen(buff), 0);
				exit(1);
			}
			buff[strlen(buff) - 1] = '\0';
		//scanf("%s", buff);
			sendBytes = send(sockfd, buff, strlen(buff), 0); //send message to server
			if(sendBytes < 0){
				perror("Error ");
				return 0;
			}
			recv(sockfd, user, BUFF_SIZE, 0);
			user[strlen(user)-2] = '\0';
			if(user[0] == 'p'){
				printf("Password:");
				fgets(buff, BUFF_SIZE, stdin);
			    buff[strlen(buff) - 1] = '\0';
			sendBytes = send(sockfd, buff, strlen(buff), 0); //send message to server
			if(sendBytes < 0){
				perror("Error ");
				return 0;
			}
			for(;;){
			recv(sockfd, user, BUFF_SIZE, 0);
			user[strlen(user)] = '\0';
			if(user[0] == 'p'){
				printf("Password:");
				fgets(buff, BUFF_SIZE, stdin);
			    buff[strlen(buff) - 1] = '\0';
			    sendBytes = send(sockfd, buff, strlen(buff), 0); //send message to server
			    if(sendBytes < 0){
				perror("Error ");
				return 0;
			}
			
			}
				if(user[0] == 'n'){
				printf("NewPassword:");
				fgets(buff, BUFF_SIZE, stdin);
			    buff[strlen(buff) - 1] = '\0';
			    sendBytes = send(sockfd, buff, strlen(buff), 0); //send message to server
			    if(sendBytes < 0){
				perror("Error ");
				return 0;
			}
			if(strcmp(buff,"bye") == 0) exit(1);
			else break;
			}
			}
			}
		strcpy(user,"user1");	
	}
	if(strcmp(user,"user2") == 0){
		for(;;){
			rcvBytes = recv(sockfd, buff, BUFF_SIZE, 0); //receive message from server
			if(rcvBytes < 0){
				perror("Error 2");
				return 0;
			}
			buff[rcvBytes] = '\0';
			if(strcmp(buff,"bye") == 0) exit(1);

			printf("Reply from server: %s\n", buff);
		}
	}
	
	close(sockfd);
	return 0;
}