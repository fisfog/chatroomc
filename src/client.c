/* client.c */

#include "chatroom.h"

#define SERVERIP "127.0.0.1"

int main(int argc, char *argv[])
{
	struct sockaddr_in servaddr;
	char buf[MAXLEN], buf2[MAXLEN];
	int sockfd;
	int n,len,flag;
	int pid;
	char servip[15+1] = SERVERIP;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &servaddr.sin_addr);
	servaddr.sin_port = htons(servip);

	if(!connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)))
		printf("Connect succeed!\n");
	else{
		printf("Cant connect to the server:%s\n", servip);
		exit(1);
	}

	pid = fork();
	if(pid<0){
		printf("fork err\n");
		exit(1);
	}else if(pid>0){
		while(scanf("%s",buf)!=EOF){
			sendMsg(sockfd, buf, strlen(buf));
		}
	}else{
		while(1){
			memset(buf2, 0x00, sizeof(buf2));
			if(recvMsg(sockfd, buf2, &len)<0){
				printf("Server [%s] closed connection\n", SERVERIP);
				break;
			}
			printf("> %s\n", buf2);
		}
	}
	close(sockfd);

	return 0;
}
