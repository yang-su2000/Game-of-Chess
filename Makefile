CXX = g++
CXXFLAGS = -std=c++14 -MMD -Wall -Werror=vla
EXEC = chess
OBJECTS = main.o chessboard.o piece.o record.o subject.o observer.o game.o window.o player.o computerOne.o human.o computerTwo.o computerThree.o computerFour.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lX11

-include ${DEPENDS}

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
.PHONY: clean
