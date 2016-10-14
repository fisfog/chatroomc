/* 
 * server functions
 * muyunlei@gmail.com
*/

#include "chatroom.h"

int
getClientCount(int mqfd)
{
	message *msg = mqMsgSTInit(NULL, 0, 1);
	if(recvMq(mqfd, msg)<=0) return -1;
	return atoi(msg->mdata);
}

int
putClientCount(int mqfd, int client_count)
{
	char cc[10] = {0};
	sprintf(cc, "%d", client_count);
	message *msg = mqMsgSTInit(cc, strlen(cc), 1);
	if(sendMq(mqfd, msg)<0) return -1;
	return 0;
}

int
login_serv(int connfd, loginfo *linfo)
{
	char buf[50] = {0};
	int len;
//	printf("DEBUG:login_serv, before recvMsg\n");
	if(recvMsg(connfd, buf, &len)<0) return -1; // recv user nick name
//	printf("DEBUG:name[%s]\n", buf);
	strcpy(linfo->login_name, buf);

	time_t t = time(NULL);
//	printf("DEBUG:login_serv, t[%d]\n", t);
	struct tm *local = localtime(&t);
	linfo->login_time = local;

	char tt[19+1] = {0};
	tm2DateTimeStr(linfo->login_time, tt);

	printf("client loginfo:\n");
	printf("\tlogin time: %s\n", tt);
	printf("\tlogin name: %s\n", linfo->login_name);

	return 0;
}
