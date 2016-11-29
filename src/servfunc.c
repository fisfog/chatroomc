/* 
 * server functions
 * muyunlei@gmail.com
*/

#include "chatroom.h"

int
getClientCount(int mqfd)
{
	int cc = 0;
	message *msg = (message *)malloc(sizeof(message));
	mqMsgSTInit(msg, NULL, 0, 1);
	if(recvMq(mqfd, msg)<=0) return -1;
	cc = atoi(msg->mdata);
	free(msg);
	return cc;
}

int
putClientCount(int mqfd, int client_count)
{
	char cc[10] = {0};
	message *msg = (message *)malloc(sizeof(message));
	snprintf(cc, sizeof(cc), "%d", client_count);
	mqMsgSTInit(msg, cc, strlen(cc), 1);
	if(sendMq(mqfd, msg)<0) return -1;
	free(msg);
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

int
broadcast2ClientsMq(int mqid, char *buf, int client_count, int curCliId, int toSelfFlag)
{
	int i;
	message *msg = (message *)malloc(sizeof(message));
	for(i=1;i<=client_count;i++){
		if(!toSelfFlag){
			if(i==curCliId) continue;
		}
		mqMsgSTInit(msg, buf, strlen(buf), 10000+i);
		sendMq(mqid, msg);
	}
	free(msg);
	return 0;
}
