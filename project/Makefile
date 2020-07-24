include ../Make.defines

PROGS =	server client 

all:	${PROGS}

server:	server.o 
		${CC} ${CFLAGS} -o $@ server.o ${LIBS} 

client:	client.o cliselect.o
		${CC} ${CFLAGS} -o $@ client.o cliselect.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
