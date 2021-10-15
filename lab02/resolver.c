#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h> 
#include <arpa/inet.h>

int get_ip(char * );

int main(int argc , char *argv[])
{
	if(argc <3)     
    {
    	printf("input error\n");         
    	exit(1);
    }
    char *temp = argv[2];
    if(strcmp(argv[1],"1")==0&&checkHostnameOrIp(temp)==0||strcmp(argv[1],"2")==0&&checkHostnameOrIp(temp)==1){
        printf("Wrong parameter\n");
    }
    else if(strcmp(argv[1],"1")==0&&checkHostnameOrIp(temp)==1)
    {
    	char *ip = argv[2];
    	get_hostname(ip);
    }
    else if(strcmp(argv[1],"2")==0&&checkHostnameOrIp(temp)==0)
    {
    	char *hostname = argv[2];
    	get_ip(hostname);
} 
}
int get_ip(char * hostname) 
{
	struct hostent *he;     
    struct in_addr **addr_list;     
    int i;     
    if ((he = gethostbyname(hostname)) == NULL)     
    {     
    	printf("Not found information.\n");
    }    
    addr_list = (struct in_addr **) he->h_addr_list;
    printf("Official IP:%s\n",inet_ntoa(*addr_list[0]));
    for(i = 1; addr_list[i] != NULL; i++)
    {
        printf("Alias IP:%s\n",inet_ntoa(*addr_list[i]));
    }
    return 1;
}

int get_hostname(char *ip)
{
	struct in_addr addr;
	struct hostent* esu;
    inet_aton(ip, &addr);
    //printf("%s\n", addr);
    esu = gethostbyaddr(((const char*)&addr),sizeof(addr), AF_INET);
    if(esu == NULL)
    {
    	printf("Not found information.\n");
    	return 1;
    }
    else{
    printf("Official HostName:%s\n",esu->h_name);
    printf("Alias HostName:%s\n",esu->h_aliases[0]);
    return 1;}
}

int checkHostnameOrIp(char *info)
{
	if(info[0] > 47 && info[0] < 58)
		{ //is IP
        return 1;}
	else
       return 0; // is Hostname
}
