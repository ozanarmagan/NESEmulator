CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c++17

SRC = $(wildcard */*.cpp) *.cpp 
OBJ = $(SRC:.cc=.o)
EXEC = test.out

all: $(EXEC) 

$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS)

clean:
	rm -rf $(OBJ) $(EXEC)