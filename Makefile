CXX=g++ -std=c++17
LIBs=-lncursesw
INC=-I./include
SRC=./src
OBJ=./obj
OBJs=$(OBJ)/GameManager.o $(OBJ)/graphics.o $(OBJ)/Board.o $(OBJ)/PlayerPane.o $(OBJ)/Player.o

all: main
test: test.test
graphictest: graphictest.test
resize: resize.test

cleanobj:
	rm obj/*.o
clean: 
	rm main *.test

$(OBJ)/GameManager.o: $(SRC)/GameManager.cc
	$(CXX) $(LIBs) $(INC) -g -c $(SRC)/GameManager.cc -o $(OBJ)/GameManager.o

$(OBJ)/graphics.o: $(SRC)/graphics.cc
	$(CXX) $(LIBs) $(INC) -c $(SRC)/graphics.cc -o $(OBJ)/graphics.o

$(OBJ)/Board.o: $(SRC)/Board.cc
	$(CXX) $(LIBs) $(INC) -g -c $(SRC)/Board.cc -o $(OBJ)/Board.o

$(OBJ)/PlayerPane.o: $(SRC)/PlayerPane.cc
	$(CXX) $(LIBs) $(INC) -g -c $(SRC)/PlayerPane.cc -o $(OBJ)/PlayerPane.o

$(OBJ)/Player.o: $(SRC)/Player.cc
	$(CXX) $(LIBs) $(INC) -g -c $(SRC)/Player.cc -o $(OBJ)/Player.o

main: $(SRC)/main.cc $(OBJs)
	$(CXX) $(LIBs) $(INC) -g -o $@ $^

test.test: test.cc
	$(CXX) $(LIBs) -o $@ $^

graphictest.test: graphictest.cc $(OBJ)/graphics.o
	$(CXX) $(LIBs) $(INC) -o $@ $^

resize.test: resizeterm.cc
	$(CXX) $(LIBs) $(INC) -o $@ $^

.PHONY: all clean cleanobj test resize graphictest
