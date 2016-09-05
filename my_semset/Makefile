test:test_sem.o comm.o
	gcc -o $@ $^

%.o:%.c
	gcc -c $<

.PHONY:clean
clean:
	rm -f test *.o
