VPATH=src
OUT=server client clientsp
OBJS=server.o util.o servfunc.o
OBJC=client.o util.o clifunc.o
OBJCSP=clientsp.o util.o clifunc.o
LFLAG=-lcurses -lpthread
OPT=-g -Wall

all: $(OUT)

server: $(OBJS)
	gcc $(OPT) $(OBJS) -o server 

client: $(OBJC)
	gcc $(OPT) $(OBJC) -o client $(LFLAG)

clientsp: $(OBJCSP)
	gcc $(OPT) $(OBJCSP) -o clientsp $(LFLAG)

server.o: chatroom.h
client.o: chatroom.h
clientsp.o: chatroom.h
util.o: chatroom.h
servfunc.o: chatroom.h
clifunc.o: chatroom.h

.PHONY:clean
clean:
	-rm -f $(OUT) *.o

.PHONY:install
install:
	-cp $(OUT) bin
