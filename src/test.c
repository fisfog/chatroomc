#include "chatroom.h"

int main()
{
	char buf[100]={"helloworld"};
	int n;
	char msg[100] = {0};
	makeAMsg(msg,buf,NORMALM);
	printf("msg[%s]\n", msg);
	memset(buf,0x00,sizeof(buf));
	parseAMsg(msg,buf,&n);
	printf("n[%d],msg[%s]\n",n,buf);
	return 0;
}
