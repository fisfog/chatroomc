/* clientsp.c */

#include "chatroom.h"

#define SERVERIP "127.0.0.1"

int nrows, ncols;

int main(int argc, char *argv[])
{
	struct sockaddr_in servaddr;
	char buf[MAXLEN], buf2[MAXLEN];
	int sockfd;
	int n,len,flag;
	int pid;
	char servip[15+1];
	int ret;

	if(argc == 2)
	{
		strcpy(servip, argv[1]);
	}else{
		printf("USAGE: client [serverip]\n");
		exit(0);
	}

	WINDOW *wnd = initscr();
	getmaxyx(wnd, nrows, ncols);

	WINDOW *logwin = newwin(0,0,0,0);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, (const char *)servip, &servaddr.sin_addr);
	servaddr.sin_port = htons(SERVPORT);

	
	ret = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if(!ret){
		wprintw(logwin,"Connect succeed!\n");
		wrefresh(logwin);
	}else{
		wprintw(logwin,"Cant connect to the server:%s\n", servip);
		wrefresh(logwin);
		exit(1);
	}
	// login
	login_cli_cgi(sockfd, logwin);

	werase(logwin);
	delwin(logwin);

	// recieve welcome message
	/*
	memset(buf, 0x00, sizeof(buf));
	recvMsg(sockfd, buf, &len);
	wprintw(winout, "%s\n", buf);
	wrefresh(winout);
	*/
	WINDOW *winin, *winout;

	pid = fork();
	if(pid<0){
		printf("fork err\n");
		exit(1);
	}else if(pid>0){
		winin = newwin(0, 0, nrows-1, 0);
		wprintw(winin, "> ");
		wrefresh(winin);
		while(!wgetnstr(winin, buf, MAXLEN)){
			sendMsg(sockfd, buf, strlen(buf));
			wprintw(winin, "> ");
			wrefresh(winin);
			wclrtoeol(winin);
		}
	}else{
		winout = newwin(nrows-2, 0, 0, 0);
		scrollok(winout, 1);
		while(1){
			memset(buf2, 0x00, sizeof(buf2));
			if(recvMsg(sockfd, buf2, &len)<0){
				wprintw(winout,"Server [%s] closed connection\n", servip);
				wrefresh(winout);
				break;
			}
			wprintw(winout, "%s\n", buf2);
			wrefresh(winout);
		}
	}
	close(sockfd);

	if(getpid()==pid)
		delwin(winin);
	else
		delwin(winout);

	endwin();
	return 0;
}
