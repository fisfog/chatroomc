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

#define MAXLEN 256
#define SERVPORT 20099
#define HEADLEN 10 + 1

#endif

ssize_t readn(int, void *, size_t);
ssize_t writen(int, void *, size_t);
int sendMsg(int, void *, int);
int recvMsg(int, void *, int *);
