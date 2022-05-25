CXX = g++ 
CCXXFLAGS =  -lSDL2 -lSDL2main -lSDL2_TTF
LDFLAGS = -O3 -std=c++17

SRC = $(wildcard */*.cpp) *.cpp 
OBJ = $(SRC:.cc=.o)

ifeq ($(OS),Windows_NT) 
	EXEC = nesemulator.exe
else
	EXEC = nesemulator.out
endif

all: $(EXEC) 

$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) -o $(EXEC) $(OBJ) $(LBLIBS) $(CCXXFLAGS)

clean:
	rm -rf $(OBJ) $(EXEC)