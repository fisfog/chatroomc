#include "chatroom.h"


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
