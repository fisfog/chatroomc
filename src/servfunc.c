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
