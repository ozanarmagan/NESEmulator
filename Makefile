CXX = g++
CCXXFLAGS =  -lSDL2 -lSDL2main

SRC = $(wildcard */*.cpp) *.cpp 
OBJ = $(SRC:.cc=.o)
EXEC = test.out

all: $(EXEC) 

$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) -o $(EXEC) $(OBJ) $(LBLIBS) $(CCXXFLAGS)

clean:
	rm -rf $(OBJ) $(EXEC)