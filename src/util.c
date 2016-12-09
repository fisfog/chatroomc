#include "chatroom.h"

/* read n bytes from socket fd buffer*/
ssize_t
readn(int fd, void *vptr, size_t n)
{
	size_t nleft;
	ssize_t nread;
	char *ptr;
	ptr = vptr;
	nleft = n;
	while(nleft > 0){
		if((nread=read(fd, ptr, nleft)) < 0){
			if(errno==EINTR)
				nread = 0;
			else
				return -1;
		}else if(nread==0)
			break;
		nleft -= nread;
		ptr += nread;
	}
	return n-nleft;
}

/* write n bytes from socket fd buffer*/
ssize_t
writen(int fd, void *vptr, size_t n)
{
	size_t nleft;
	ssize_t nwritten;
	char *ptr;
	ptr = vptr;
	nleft = n;
	while(nleft>0){
		if((nwritten = write(fd, ptr, n))<=0){
			if(nwritten < 0 && errno == EINTR)
				nwritten = 0;
			else
				return -1;
		}
		nleft -= nwritten;
		ptr += nwritten;
	}
	return n-nleft;
}

/* 
 * send a msg to socket fd
 * with msg head
*/
int
sendMsg(int fd, void *vptr, int n)
{
	char head[HEADLEN];
	char *ptr = vptr;
	memset(head, 0x00, sizeof(head));
	snprintf(head, sizeof(head), "%010d", n);
	if((writen(fd, head, sizeof(head)))!=sizeof(head)) return -1;
	if((writen(fd, ptr, n))!=n) return -1;
	return 0;
}

/*
 * recieve a msg from socket fd
*/

int
recvMsg(int fd, void *vptr, int *n)
{
	char head[HEADLEN];
	char *ptr = vptr;
	memset(head, 0x00, sizeof(head));
	if((readn(fd, head, sizeof(head)))!=sizeof(head)) return -1;
	*n = atoi(head);
	if((readn(fd, ptr, *n))!=*n) return -1;
	return 0;
}

/*
 * Init a SYSV MQ struct
*/
int
mqMsgSTInit(message *msg, char *data, long len, long type)
{
	memset(msg->mdata, 0x00, sizeof(msg->mdata));
	if(data!=NULL) strcpy(msg->mdata, data);
	msg->mlen = len;
	msg->mtype = type;
	return 0;
}

/*
 * send a mq msg to mq[id]
*/
ssize_t
sendMq(int id, message *mptr)
{
	return msgsnd(id, &(mptr->mtype), MAXLEN, 0);
}

/*
 * recv a mq msg from mq[id]
*/
ssize_t
recvMq(int id, message *mptr)
{
	ssize_t n = msgrcv(id, &(mptr->mtype), MAXLEN, mptr->mtype, 0);
	mptr->mlen = n;
	return n;
}

/*
 * get local time
 */

/*
 * DataTime String with format: "YYYY-MM-DD HH:MM:SS"
 */

int
tm2DateTimeStr(struct tm *t, char *dts)
{
	snprintf(dts,
		sizeof(dts),
		"%04d-%02d-%02d %02d:%02d:%02d",
		t->tm_year+1900,
		t->tm_mon+1,
		t->tm_mday,
		t->tm_hour,
		t->tm_min,
		t->tm_sec);
//	printf("DEBUG:tm2DateTimeStr, dts[%s]\n", dts);
	return 0;
}

/*
 * get current time string
 * format: YYYY-MM-DD HH:MM:SS
 */

int
getCurTimeStr(char *cts)
{
	time_t t = time(NULL);
	struct tm *local = localtime(&t);
	tm2DateTimeStr(local, cts);
	return 0;
}

/*
 * make a msg with msg type
 * like: msg: "hello" msg type: NORMALM
 * ----> 1001hello
*/
int
makeAMsg(char *msg, char *buf, int msgType)
{
	memset(msg,0x00,sizeof(msg));
	snprintf(msg, sizeof(msg), "%04d%s", msgType, buf);
	return 0;
}

/*
 * parse a msg -> msg type + msg buf
*/
int
parseAMsg(char *msg, char *buf, int *msgType)
{
	int i;
	char str[MAXLEN+1];
	for(i=0;i<4;i++)
		memset(&str[i],msg[i],1);
	*msgType = atoi(str);
	memset(str, 0x00, sizeof(str));
	for(i=0;i<strlen(msg);i++)
		memset(&str[i],msg[i+4],1);
	strcpy(buf, str);
	return 0;
}

/*
 * malloc a char 2D array
*/
char **
malloc2dCharArray(int rows, int cols)
{
	char **ptr;
	int i;
	ptr = (char **)malloc(sizeof(char *)*rows);
	for(i=0;i<rows;i++)
		ptr[i] = (char *)malloc(sizeof(char)*cols);
	return ptr;
}

/*
 * free a char 2D array
 */
void
free2dCharArray(char **ptr, int rows)
{
	int i;
	for(i=0;i<rows;i++)
		free(ptr[i]);
	free(ptr);

}

void
sig_chld(int signo)
{
	pid_t	pid;
	int 	stat;
	while((pid = waitpid(-1, &stat, WNOHANG))>0)
		printf("child %d terminated\n", pid);
	return;
}

