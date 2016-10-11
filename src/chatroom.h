/* chatroom.h
 * muyunlei@gmail.com
 */


#ifndef __CHATROOM_H
#define __CHATROOM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <error.h>

/* socket lib */
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLEN 256
#define SERVPORT 20099

#endif

ssize_t readn(int, void *, size_t);
ssize_t writen(int, void *, size_t);

