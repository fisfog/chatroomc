/* chatroom.h
 * muyunlei@gmail.com
 */


#ifndef __CHATROOM_H
#define __CHATROOM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

/* socket lib */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* ipc lib */
#include <sys/ipc.h>
#include <sys/msg.h>

#define SERVPORT 20099

#define MAXLEN 256
#define HEADLEN 10 + 1

#define MSG_R 0400
#define MSG_W 0200
#define SVMSG_MODE (MSG_R | MSG_W | MSG_R >> 3 | MSG_R >> 6)

typedef struct mqmesg{
	long mtype;
	long mlen;
	char mdata[MAXLEN];
}message;

#endif


/* util.c */
ssize_t readn(int, void *, size_t);
ssize_t writen(int, void *, size_t);
int sendMsg(int, void *, int);
int recvMsg(int, void *, int *);
message *mqMsgSTInit(char *, long, long);
ssize_t sendMq(int, message *);
ssize_t recvMq(int, message *);

/* servfunc.c */
int getClientCount(int);
int putClientCount(int, int);
