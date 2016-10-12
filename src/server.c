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

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVPORT);

	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	listen(listenfd, 20);

	while(1){
		cliaddr_len = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr_in *)&cliaddr, &cliaddr_len);
		pid = fork();
		if(pid)
	}
	return 0;
}
