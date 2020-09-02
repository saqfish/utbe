CC = gcc -g
OBJS = utbe.o util.o parse.o
LDLIBS = -ljansson

utbe: $(OBJS)
	$(CC) $(OBJS) $(LDLIBS) $(LDFLAGS) -o utbe

utbe.o: utbe.c utbe.h 
	$(CC) $(CFLAGS) -c utbe.c

util.o: util.c util.h 
	$(CC) -c util.c 

parse.o: parse.c parse.h 
	$(CC) -c parse.c 

clean: 
	rm *.o utbe
