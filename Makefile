CC = gcc -std=c99
OBJS = utbe.o util.o parse.o fetch.o output.o
LDLIBS = -ljansson

utbe: $(OBJS)
	$(CC) $(OBJS) $(LDLIBS) $(LDFLAGS) -o utbe

utbe.o: utbe.c utbe.h 
	$(CC) $(CFLAGS) -c utbe.c

util.o: util.c util.h 
	$(CC) -c util.c 

parse.o: parse.c parse.h 
	$(CC) -c parse.c 

fetch.o: fetch.c fetch.h 
	$(CC) -c fetch.c 

output.o: output.c output.h 
	$(CC) -c output.c 

clean: 
	rm *.o utbe
