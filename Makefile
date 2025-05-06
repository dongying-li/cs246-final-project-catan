CXX = g++
CXXFLAGS = -std=c++14 -g -Wall -MMD
OBJECTS = enum.o board.o builder.o catan.o dice.o edge.o fairdice.o geese.o inventory.o loadeddice.o main.o subject.o tile.o vertex.o
DEPENDS = ${OBJECTS:.o=.d}
EXEC = constructor

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

clean :
	rm ${DEPENDS} ${OBJECTS} ${EXEC}

-include ${DEPENDS} # reads the .d files and reruns dependencies
