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
