OBJ=server client
OBJS=server.o util.o servfunc.o
OBJC=client.o util.o

all: $(OBJ)

server: $(OBJS)
	gcc $(OBJS) -o server

client: $(OBJC)
	gcc $(OBJC) -o client

server.o: server.c chatroom.h
	gcc -c server.c 

client.o: client.c chatroom.h
	gcc -c client.c

util.o: util.c chatroom.h
	gcc -c util.c

servfunc.o: servfunc.c chatroom.h
	gcc -c servfunc.c

clean:
	-rm server client *.o
