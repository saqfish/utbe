CC = gcc -g
OBJS = utbe.o util.o 
LDLIBS = -l json-c 
LDFLAGS = -L /lib
CFLAGS = -I /usr/include/json-c/ 

utbe: $(OBJS)
	$(CC) $(OBJS) $(LDLIBS) $(LDFLAGS) -o utbe

utbe.o: utbe.c utbe.h 
	$(CC) $(CFLAGS) -c utbe.c

util.o: util.c util.h
	$(CC) -c util.c 

clean: 
	rm *.o utbe
