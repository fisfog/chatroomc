/* clientsp.c */

#include "chatroom.h"

int nrows, ncols;
pthread_t ntid;


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

//	wprintw(logwin, "DEBUG: servip[%s]\n", servip);
//	wrefresh(logwin);

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
		sleep(1);
		delwin(logwin);
		endwin();
		return -1;
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
	WINDOW *winin, *winout, *winulist;
	winin = newwin(0, 0, nrows-1, 0);
	winout = newwin(nrows-2, 0, 0, 0);
	winulist = newwin(nrows-2, 0, 0, ncols-20);
	scrollok(winout, 1);
	scrollok(winulist, 1);

	thrarg ta = {winout, winulist, sockfd, servip};

	ret = pthread_create(&ntid, NULL, (void *)thr_fn, &ta);
	if(ret != 0){
		wprintw(winout, "cant create thread\n");
		wrefresh(winout);
		close(sockfd);
		delwin(winin);
		delwin(winout);
		delwin(winulist);
		exit(ret);
	}

	wprintw(winin, "> ");
	wrefresh(winin);
	while(!wgetnstr(winin, buf, MAXLEN)){
		sendMsg(sockfd, buf, strlen(buf));
		wclrtoeol(winin);
		wprintw(winin, "> ");
		wrefresh(winin);
	}
	
	close(sockfd);
	delwin(winin);
	delwin(winout);
	delwin(winulist);

	endwin();
	return 0;
}
