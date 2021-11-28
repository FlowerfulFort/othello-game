CXX=g++ -std=c++17
LIBs=-lncursesw
INC=-I./include
SRC=./src
OBJ=./obj

all: main
test: test.test
graphictest: graphictest.test
resize: resize.test

cleanobj:
	rm obj/*.o
clean: 
	rm main *.test

$(OBJ)/GameManager.o: $(SRC)/GameManager.cc
	$(CXX) $(LIBs) $(INC) -c $(SRC)/GameManager.cc -o $(OBJ)/GameManager.o

$(OBJ)/graphics.o: $(SRC)/graphics.cc
	$(CXX) $(LIBs) $(INC) -c $(SRC)/graphics.cc -o $(OBJ)/graphics.o

main: $(SRC)/main.cc $(OBJ)/GameManager.o
	$(CXX) $(LIBs) $(INC) -o $@ $^

test.test: test.cc
	$(CXX) $(LIBs) -o $@ $^

graphictest.test: graphictest.cc $(OBJ)/graphics.o
	$(CXX) $(LIBs) $(INC) -o $@ $^

resize.test: resizeterm.cc
	$(CXX) $(LIBs) $(INC) -o $@ $^

.PHONY: all clean cleanobj test resize graphictest
