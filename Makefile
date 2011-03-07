OBJS = GameState.o Agents.o Utils.o
CC = G++
DEBUG = -g
CFLAGS = -I/opt/local/include/ -c ${DEBUG}
LFLAGS = -I/opt/local/include/ ${DEBUG} -lsfml-graphics -lsfml-window -lsfml-system

rr: $(OBJS) main.cpp
	${CC} ${LFLAGS} ${OBJS} -o rr main.cpp

GameState.o: GameState.cpp GameState.h Constants.h
	${CC} ${CFLAGS} GameState.cpp

Agents.o: Agents.cpp Agents.h GameState.h Constants.h Utils.h
	${CC} ${CFLAGS} Agents.cpp

Utils.o: Utils.cpp Utils.h GameState.h Constants.h
	${CC} ${CFLAGS} Utils.cpp

clean:
	rm *.o *~ *.gch rr

run: rr
	./rr Levels/default
