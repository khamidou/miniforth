OBJS=vm.o main.o fail.o
CFLAGS=-c -g

vm_and_as: vm as

as: as.o lexer fail.o
	$(CC) as.o aslexer.o fail.o -o as -lfl

as.o: as.c
	$(CC) $(CFLAGS) as.c

lexer: aslexer.l
	lex -i -o aslexer.c aslexer.l
	$(CC) -c aslexer.c

vm: $(OBJS)
	$(CC) $(OBJS) -o vm

vm.o: vm.c vm.h
	$(CC) $(CFLAGS) vm.c

main.o: main.c
	$(CC) $(CFLAGS) main.c


fail.o: fail.c fail.h
	$(CC) $(CFLAGS) fail.c

clean:
	rm *.o
	rm as vm
