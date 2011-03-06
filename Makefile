OBJS = GameState.o Agents.o Utils.o
CC = G++
DEBUG = -g
CFLAGS = -Wall -I/opt/local/include/ -c ${DEBUG}
LFLAGS = -Wall -I/opt/local/include/ ${DEBUG}

rr: $(OBJS) main.cpp
	${CC} ${LFLAGS} ${OBJS} -o rr main.cpp

clean:
	rm *.o *~ *.gch rr

run: rr
	./rr Levels/test3
