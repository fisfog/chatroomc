/* server.c
* muyunlei@gmail.com
*/

#include "chatroom.h"

int main(int argc, char *argv[])
{
	struct sockaddr_in servaddr, cliaddr;
	socklen_t cliaddr_len;
	char buf[MAXLEN+1];
	char addr[INET_ADDRSTRLEN];
	int listenfd,connfd;
	int i,n,len;
	int pid;
	int client_count = 0;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVPORT);

	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	listen(listenfd, 20);

	while(1){
		cliaddr_len = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
		client_count++;
		pid = fork();
		if(pid<0) printf("fork err\n");
		else if(pid>0){
			continue;
		}else{
			// child
			printf("this is child process[%d]\n", getpid());
			inet_ntop(AF_INET, &cliaddr.sin_addr, addr, sizeof(addr));
			printf("Recieved connection form [%s] at PORT [%d]\n", addr, ntohs(cliaddr.sin_port));
			char welcome[100];
			memset(welcome, 0x00, sizeof(welcome));
			sprintf(welcome, "%s%d%s", "-----welcome to chat room, current user no: ", client_count, "------");
			sendMsg(connfd, welcome, strlen(welcome));
			while(1){
				memset(buf, 0x00, sizeof(buf));
				if(recvMsg(connfd, buf, &len)<0){
					printf("The client [%d] closed the connection.\n", getpid());
					break;
				}
				printf("recv[%d]:[%s]\n", len, buf);

				strcat(buf, "[B]");
//				printf("DEBUG: [%d], buf[%s]\n", strlen(buf), buf);
				sendMsg(connfd, buf, strlen(buf));
			}
			close(connfd);
			break;
		}
	}
	close(listenfd);
	return 0;
}
