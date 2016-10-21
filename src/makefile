OBJ=server client clientsp
OBJS=server.o util.o servfunc.o
OBJC=client.o util.o clifunc.o
OBJCSP=clientsp.o util.o clifunc.o
LFLAG=lcurses

all: $(OBJ)

server: $(OBJS)
	gcc $(OBJS) -o server 

client: $(OBJC)
	gcc $(OBJC) -o client -$(LFLAG)

clientsp: $(OBJCSP)
	gcc $(OBJCSP) -o clientsp -$(LFLAG)

server.o: server.c chatroom.h
	gcc -c server.c 

client.o: client.c chatroom.h
	gcc -c client.c

clientsp.o: clientsp.c chatroom.h
	gcc -c clientsp.c

util.o: util.c chatroom.h
	gcc -c util.c

servfunc.o: servfunc.c chatroom.h
	gcc -c servfunc.c

clifunc.o: clifunc.c chatroom.h
	gcc -c clifunc.c -$(LFLAG)

clean:
	-rm server client clientsp *.o
