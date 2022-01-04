CXX = g++ 
CCXXFLAGS =  -lSDL2 -lSDL2main
LDFLAGS = -O3 -std=c++17

SRC = $(wildcard */*.cpp) *.cpp 
OBJ = $(SRC:.cc=.o)
EXEC = test.exe

all: $(EXEC) 

$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) -o $(EXEC) $(OBJ) $(LBLIBS) $(CCXXFLAGS)

clean:
	rm -rf $(OBJ) $(EXEC)