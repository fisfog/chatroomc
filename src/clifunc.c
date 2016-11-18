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

int
parseNotifyMsg(char *msg, char **infoArray)
{
	char *p;
	p = strtok(msg, "|");
	int i = 0;
	while(p!=NULL){
		strcpy(infoArray[i], p);
		p = strtok(NULL, "|");
		i++;
	}
	return 0;
}

void
printUserList(char **userNameList, WINDOW *wnd)
{
	int i;
	for(i=0;i<ORIUSERNUM;i++){
		if(userNameList[i]!="")
		{
			wprintw(wnd, "%s\n", userNameList[i]);
		}
	}
	wrefresh(wnd);
}

void *
thr_fn(thrarg *arg)
{
	char msgbuf[MAXLEN + 1];
	char buf[MAXLEN + 1] = {0};
	int msgType;
	int len;
	char **infoArray;
	char **userNameList;
	int i, userIdx;
	// malloc 2D infoArray
	infoArray = malloc2dCharArray(NOTIFYINFONUM, NOTIFYINFOLEN);
	userNameList = malloc2dCharArray(ORIUSERNUM, NAMELEN);
	while(1){
		memset(msgbuf, 0x00, sizeof(msgbuf));
		if(recvMsg(arg->socket, msgbuf, &len)<0){
			wprintw(arg->wnd_output, "Server [%s] closed connection\n", arg->servip);
			wrefresh(arg->wnd_output);
			break;
		}
		memset(buf,0x00,sizeof(buf));
		parseAMsg(msgbuf,buf,&msgType);
		for(i=0;i<NOTIFYINFONUM;i++)
			memset(infoArray[i], 0x00, sizeof(infoArray[i]));
		if(msgType/10==200){
			parseNotifyMsg(buf, infoArray);
			userIdx = atoi(infoArray[0]);
			if(msgType==NOTIFYM_JOIN){
				strcpy(userNameList[userIdx], infoArray[2]);
				sprintf(buf,"(%s) %s join the chatroom\n", infoArray[1], infoArray[2]);
			}
			if(msgType==NOTIFYM_QUIT){
				memset(userNameList[userIdx],0x00,sizeof(userNameList[userIdx]));
				sprintf(buf,"(%s) %s quit the chatroom\n", infoArray[1], infoArray[2]);
			}
			printUserList(userNameList, arg->wnd_ulist);
		}
		wprintw(arg->wnd_output, "%s\n", buf);
		wrefresh(arg->wnd_output);
	}
	free2dCharArray(infoArray, NOTIFYINFONUM);
	free2dCharArray(userNameList, ORIUSERNUM);
	return ((void *)0);
}
