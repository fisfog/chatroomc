/* server.c
* muyunlei@gmail.com
*/

#include "chatroom.h"

int main(int argc, char *argv[])
{
	struct sockaddr_in servaddr, cliaddr;
	socklen_t cliaddr_len;
	char buf[MAXLEN+1];
	char buf2[MAXLEN] = {0};
	char msgbuf[MAXLEN] = {0};
	char addr[INET_ADDRSTRLEN];
	int listenfd,connfd;
	int i,n,len;
	int pid;
	int client_count = 0;
	int fpid = getpid();
	char tt[19+1] = {0};


	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVPORT);

	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	listen(listenfd, 20);
	printf("Accepting connect...\n");

	int mq_fd = msgget(IPC_PRIVATE, SVMSG_MODE | IPC_CREAT);
	printf("msgid:%d\n",mq_fd);
	putClientCount(mq_fd, client_count);
	message *msg = (message *)malloc(sizeof(message));

	while(1){
		cliaddr_len = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
		client_count = getClientCount(mq_fd);	
		client_count++;
		putClientCount(mq_fd, client_count);
		pid = fork();
		if(pid<0) printf("fork err\n");
		else if(pid>0){
			continue;
		}else{
			// child
			printf("this is child process[%d]\n", getpid());
			inet_ntop(AF_INET, &cliaddr.sin_addr, addr, sizeof(addr));
			printf("Recieved connection form [%s] at PORT [%d]\n", addr, ntohs(cliaddr.sin_port));
			client_count = getClientCount(mq_fd);	
			int cliNo = client_count;
			putClientCount(mq_fd, client_count);

			loginfo *cli_log_info = (loginfo *)malloc(sizeof(loginfo));
			cli_log_info->cliaddr = &cliaddr;
			login_serv(connfd, cli_log_info); // client login

			getCurTimeStr(tt);
			sprintf(buf2, "%d|%s|%s", cliNo, tt, cli_log_info->login_name);
			makeAMsg(msgbuf,buf2,NOTIFYM_JOIN);
			client_count = getClientCount(mq_fd);
			putClientCount(mq_fd, client_count);

			broadcast2ClientsMq(mq_fd, msgbuf, client_count, cliNo, 0);

			char welcome[100] = {0};
			sprintf(welcome, "%s%d%s", "-----welcome to chat room, current user no: ", client_count, "------");
			makeAMsg(msgbuf,welcome,NORMALM);
			sendMsg(connfd, msgbuf, strlen(msgbuf));

			int pid2 = fork();
			if(pid2<0){ printf("fork err\n"); continue;}
			else if(pid2>0){
				while(1){
					memset(buf, 0x00, sizeof(buf));
					if(recvMsg(connfd, buf, &len)<0){
						printf("The client [%d] closed the connection.\n", getpid());

						getCurTimeStr(tt);
						sprintf(buf2, "%d|%s|%s", cliNo, tt, cli_log_info->login_name);
						makeAMsg(msgbuf,buf2,NOTIFYM_QUIT);
						client_count = getClientCount(mq_fd);
						putClientCount(mq_fd, client_count);

						broadcast2ClientsMq(mq_fd, msgbuf, client_count, cliNo, 0);

						kill(pid2, SIGKILL);
						break;
					}
					printf("CLIENT[%s]:PID[%d]:LOGIN_NAME[%s]:LEN[%d]:MSG[%s]\n", addr, getpid(), cli_log_info->login_name, len, buf);
					
					getCurTimeStr(tt);
					
					sprintf(buf2, "(%s) %s: %s", tt, cli_log_info->login_name, buf);
					makeAMsg(msgbuf,buf2,NORMALM);
					client_count = getClientCount(mq_fd);
					putClientCount(mq_fd, client_count);

					broadcast2ClientsMq(mq_fd, msgbuf, client_count, cliNo, 1);

				}
			}else{
				while(1){
					mqMsgSTInit(msg, NULL, 0, 10000+cliNo);
					if(recvMq(mq_fd, msg)<=0) continue;
					else{
						sendMsg(connfd, msg->mdata, msg->mlen);
					}
				}
			}
			client_count = getClientCount(mq_fd);
			client_count--;
			putClientCount(mq_fd, client_count);
			close(connfd);
			break;
		}
	}
	close(listenfd);
	free(msg);
	if(getpid() == fpid){
		msgctl(mq_fd, IPC_RMID, NULL);
	}

	return 0;
}
