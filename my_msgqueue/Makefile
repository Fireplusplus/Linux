ROOT_PATH=$(shell pwd)
COMM_PATH=$(ROOT_PATH)/comm
INCLUDE=-I$(COMM_PATH)

.PHONY:all
all:server client


server:server.o comm.o
	@gcc -o $@ $^ $(INCLUDE)
client:client.o comm.o
	@gcc -o $@ $^ $(INCLUDE)

%.o:$(ROOT_PATH)/%.c
	@gcc -c $(<) $(INCLUDE)
%.o:$(COMM_PATH)/%.c
	@gcc -c $(<) $(INCLUDE)

.PHONY:clean
clean:
	rm -rf server client *.o
