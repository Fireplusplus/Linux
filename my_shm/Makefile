.PHONY:all
all:server client

server:server.o comm.o
	gcc -o $@ $^
client:client.o comm.o
	gcc -o $@ $^

%.o:%.c
	gcc -c $<

.PHONY:clean
clean:
	rm -rf client server *.o
