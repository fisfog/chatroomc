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
#include <signal.h>
#include <time.h>
#include <sys/wait.h>

/* socket lib */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* ipc lib */
#include <sys/ipc.h>
#include <sys/msg.h>

/* curses lib */
#include <curses.h>

/* pthread lib */
#include <pthread.h>


#define SERVPORT 20123

#define MAXLEN 256
#define HEADLEN 10 + 1

#define MSG_R 0400
#define MSG_W 0200
#define SVMSG_MODE (MSG_R | MSG_W | MSG_R >> 3 | MSG_R >> 6)


#define NORMALM 1001
#define NOTIFYM_JOIN 2001
#define NOTIFYM_QUIT 2002


#define NOTIFYINFONUM 3
#define NOTIFYINFOLEN 20 + 1
#define NAMELEN 50 + 1
#define ORIUSERNUM 100

/* SYSV MQ message st */
typedef struct mqmesg{
	long mtype;
	long mlen;
	char mdata[MAXLEN];
}message;

/* user login info st */
typedef struct lginfo{
	struct tm		*login_time; // login time st
	struct sockaddr_in	*cliaddr; // client IP4 addr st
	char			login_name[NAMELEN]; // user input nick name
}loginfo;

/* thread func arg st*/
typedef struct thr_arg{
	WINDOW	*wnd_output;
	WINDOW	*wnd_ulist;
	int	socket;
	char	*servip;
}thrarg;


/* util.c */
ssize_t readn(int, void *, size_t);
ssize_t writen(int, void *, size_t);
int sendMsg(int, void *, int);
int recvMsg(int, void *, int *);
int mqMsgSTInit(message *, char *, long, long);
ssize_t sendMq(int, message *);
ssize_t recvMq(int, message *);
int tm2DateTimeStr(struct tm *, char *);
int getCurTimeStr(char *);
int makeAMsg(char *, char *, int);
int parseAMsg(char *, char *, int *);
char **malloc2dCharArray(int, int);
void free2dCharArray(char **, int);
void sig_chld(int);

/* servfunc.c */
int getClientCount(int);
int putClientCount(int, int);
int login_serv(int, loginfo *);
int broadcast2ClientsMq(int, char *, int, int, int);


/* clifunc.c */
int login_cli(int);
int login_cli_cgi(int, WINDOW *);
void *thr_fn(thrarg *);

#endif
