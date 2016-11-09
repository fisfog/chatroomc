/* clifunc.c
*/

#include "chatroom.h"

int
login_cli(int connfd)
{
	char nname[50] = {0};
	printf("please input a nick name: ");
	fgets(nname, 50, stdin);
	memset(&nname[strlen(nname)-1],0,1);
	sendMsg(connfd, nname, strlen(nname));
	return 0;
}

int
login_cli_cgi(int connfd, WINDOW *wp)
{
	char nname[50] = {0};
	wprintw(wp, "please input a nick name: ");
	wrefresh(wp);
	wgetnstr(wp, nname, 50);
	sendMsg(connfd, nname, strlen(nname));
	return 0;
}

void *
thr_fn(thrarg *arg)
{
	char msgbuf[MAXLEN + 1];
	char buf[MAXLEN + 1] = {0};
	int msgType;
	int len;
	while(1){
		memset(msgbuf, 0x00, sizeof(msgbuf));
		if(recvMsg(arg->socket, msgbuf, &len)<0){
			wprintw(arg->wnd, "Server [%s] closed connection\n", arg->servip);
			wrefresh(arg->wnd);
			break;
		}
		memset(buf,0x00,sizeof(buf));
		parseAMsg(msgbuf,buf,&msgType);
		wprintw(arg->wnd, "%s\n", buf);
		wrefresh(arg->wnd);
	}
	return ((void *)0);
}
