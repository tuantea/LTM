
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#define BUFF_SIZE 255

char number[BUFF_SIZE];
char alphabet[BUFF_SIZE];
int error = 0;
struct user{
    char* username;
    char* password;
    int status;
    int bloked;
    struct user *next;
    int signin;
};
typedef struct user USER;
USER* Readfile(char fileName[])
   { FILE *fp;
    char line[128];
    USER *current,*head;
    head=current=NULL;
    fp = fopen(fileName, "r");

    while(fgets(line, sizeof(line), fp)){
        USER *node = malloc(sizeof(USER));
        char *string1=strtok(line," ");
        node->username = strdup(string1);//note : strdup is not standard function
        string1 = strtok(NULL, " ");
        node->password=strdup(string1);
        string1 = strtok(NULL, " ");
        node->status=atoi(string1);
        node->bloked=0;
        node->signin=0;
        node->next =NULL;

        if(head == NULL){
            current = head = node;
        } else {
            current = current->next = node;
        }
    }
    fclose(fp);
    //test print
    for(current = head; current ; current=current->next){
        printf("%s %s %d\n", current->username,current->password,current->status);
    }
    //need free for each node
    return head;
}
int testuser(USER *head,char username[])
{
    USER *p;
    for(p=head;p;p=p->next)
    {   
        if(strcmp(p->username,username)==0)
        {
         return 0;
        }
    }
    return 1;
}
void processRecvBuff(char *receive){	//process message from client with ASCII
	int countNumber = 0, countAlphabet = 0;
	memset(number,'\0',sizeof(number));
	memset(alphabet,'\0', sizeof(alphabet));
	for (int i = 0; i < strlen(receive); i++)
	{
		if(receive[i] > 47 && receive[i] < 58){
			number[countNumber] = receive[i];
			countNumber ++;
		}
		else 
			if((receive[i] > 64 && receive[i] < 91) || (receive[i] > 96 && receive[i] < 123)){
				alphabet[countAlphabet] = receive[i];
				countAlphabet ++;
			}
			else {
				error = 1;
				break;
			}
	}
}

int main(int argc, char *argv[])
{
	int sockfd, rcvSize,i=0;
	unsigned int len;
	char buff[BUFF_SIZE],username[20],password[20];
	char recvBuff[BUFF_SIZE];
	char done[BUFF_SIZE];
	struct sockaddr_in servaddr, cliaddr1, cliaddr2;
	short serv_PORT;
	USER *head=Readfile("user.txt");
	USER *p=head;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <UDP SERVER PORT>\n", argv[0]);
		exit(1);
	}
	serv_PORT = atoi(argv[1]);
//Step 1: Construct socket
	if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
		perror("Error: ");
		return 0;
	}
//Step 2: Bind address to socket
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(serv_PORT);

	if(bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
		perror("Error: ");
		return 0;
	}
	printf("Server started!\n");

	len = sizeof(cliaddr1);
	recvfrom(sockfd, recvBuff, BUFF_SIZE, 0,(struct sockaddr *) &cliaddr1, &len); //connect with client1
	strcpy(buff,"user1");
	sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *) &cliaddr1, sizeof(cliaddr1));

	len = sizeof(cliaddr2);
	recvfrom(sockfd, recvBuff, BUFF_SIZE, 0,(struct sockaddr *) &cliaddr2, &len); //connect with client2
	strcpy(buff,"user2");
	sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *) &cliaddr2, sizeof(cliaddr2));


	
//Step 3: Communicate with client
	for ( ; ; )
	{
		memset(buff,'\0', sizeof(buff));
		memset(done,'\0', sizeof(done));
		memset(recvBuff, '\0', sizeof(recvBuff));
		len = sizeof(cliaddr1);
		rcvSize = recvfrom(sockfd, recvBuff, BUFF_SIZE, 0,(struct sockaddr *) &cliaddr1, &len); //receive message from client
		if(rcvSize < 0){
			perror("Error: ");
			return 0;
		}
		recvBuff[rcvSize] = '\0';
        strcpy(username,recvBuff);
		printf("Receive from client: %s\n", username);
		if(testuser(head,username)==1){
			strcpy(buff,"user");
	        sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *) &cliaddr1, sizeof(cliaddr1));
		    printf("Not OK\n");

		}
		if(testuser(head,username)==0){
			strcpy(buff,"password");
	        sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *) &cliaddr1, sizeof(cliaddr1));
			rcvSize = recvfrom(sockfd, recvBuff, BUFF_SIZE, 0,(struct sockaddr *) &cliaddr1, &len); //receive message from client
		    if(rcvSize < 0){
			perror("Error: ");
			return 0;
		}
		recvBuff[rcvSize] = '\0';
        strcpy(password,recvBuff);
		while(p!=NULL&&(strcmp(p->username,username)!=0)){
        p=p->next;
    }
    while(strcmp(p->password,password)!=0){
        p->bloked++;
        if(p->bloked>=3){
            p->status=0;
            printf("Password is incorrect.\nAccount is blocked.\n");
			i=3;
        }
        else{
            printf("Password is incorrect.\n");
        }
		strcpy(buff,"password");
	    sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *) &cliaddr1, sizeof(cliaddr1));
		rcvSize = recvfrom(sockfd, recvBuff, BUFF_SIZE, 0,(struct sockaddr *) &cliaddr1, &len); //receive message from client
		    if(rcvSize < 0){
			perror("Error: ");
			return 0;
		}
		recvBuff[rcvSize] = '\0';
        strcpy(password,recvBuff);
    }
    if(strcmp(p->password,password)==0) {
		if(p->status==1){
		printf("OK\n");
		strcpy(buff,"newpassword");
	    sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *) &cliaddr1, sizeof(cliaddr1));
	   }
		else{
			printf("account not ready\n");
		 }
		i=3;
    }
		
		
        rcvSize = recvfrom(sockfd, recvBuff, BUFF_SIZE, 0,(struct sockaddr *) &cliaddr1, &len); //receive message from client
		    if(rcvSize < 0){
			perror("Error: ");
			return 0;
		   }
		recvBuff[rcvSize] = '\0';
        strcpy(password,recvBuff);
		processRecvBuff(recvBuff);
		if(error == 1){
			strcpy(buff,"error");
			sendto(sockfd, buff, strlen(buff), 0,(struct sockaddr *) &cliaddr2, sizeof(cliaddr2)); //error message
			error = 0;
		    }
		else{
			if(strcmp(recvBuff,"bye") == 0) {
				strcpy(buff,"bye");
			    sendto(sockfd, buff, strlen(buff), 0,(struct sockaddr *) &cliaddr2, sizeof(cliaddr2));
				exit(1);}
			if(number[0] != '\0')
				sendto(sockfd, number, rcvSize, 0,(struct sockaddr *) &cliaddr2, sizeof(cliaddr2));
			if(alphabet[0] != '\0')
				sendto(sockfd, alphabet, rcvSize, 0,(struct sockaddr *) &cliaddr2, sizeof(cliaddr2));
		   }
		}
	}
	return 0;
}