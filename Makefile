#
# Makefile for quiz_server and quiz_client
#
OBJS=main.o cpuboard.o

all: main

main: ${OBJS}
	${CC} ${CFLAGS} -o $@ $^

clean:
	${RM} *.o main *~