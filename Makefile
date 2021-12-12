CXX=g++ -std=c++17
LIBs=-lncursesw
INC=-I./include
SRC=./src
OBJ=./obj
OBJs=$(OBJ)/GameManager.o $(OBJ)/graphics.o $(OBJ)/Board.o $(OBJ)/PlayerPane.o $(OBJ)/Player.o
TESTSRC=./src/test
TESTSRCs=$(TESTSRC)/Board.cpp $(TESTSRC)/Game.cpp $(TESTSRC)/GameManager.cpp $(TESTSRC)/Position.cpp
TESTHDR=./include/test
TESTOBJs=testobj/Board.o testobj/Game.o testobj/GameManager.o testobj/Position.o
GTEST_DIR=./googletest/googletest
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
CXXFLAGS += -pthread
CPPFLAGS += -isystem $(GTEST_DIR)/include
all: directory main
test: testdir mktest

directory:
	mkdir -p obj
testdir:
	mkdir -p testobj

clean: 
	rm main obj/*.o
cleantest:
	rm testobj/* test
$(OBJ)/GameManager.o: $(SRC)/GameManager.cc
	$(CXX) $(INC) -c $(SRC)/GameManager.cc -o $(OBJ)/GameManager.o $(LIBs)

$(OBJ)/graphics.o: $(SRC)/graphics.cc
	$(CXX) $(INC) -c $(SRC)/graphics.cc -o $(OBJ)/graphics.o $(LIBs)

$(OBJ)/Board.o: $(SRC)/Board.cc
	$(CXX) $(INC) -c $(SRC)/Board.cc -o $(OBJ)/Board.o $(LIBs)

$(OBJ)/PlayerPane.o: $(SRC)/PlayerPane.cc
	$(CXX) $(INC) -c $(SRC)/PlayerPane.cc -o $(OBJ)/PlayerPane.o $(LIBs)

$(OBJ)/Player.o: $(SRC)/Player.cc
	$(CXX) $(INC) -c $(SRC)/Player.cc -o $(OBJ)/Player.o $(LIBs)

main: $(SRC)/main.cc $(OBJs)
	$(CXX) $(INC) -o $@ $^ $(LIBs)

testobj/gtest-all.o : $(GTEST_SRCS_)
	g++ $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
        $(GTEST_DIR)/src/gtest-all.cc -o $@

testobj/gtest_main.o : $(GTEST_SRCS_)
	g++ $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
        $(GTEST_DIR)/src/gtest_main.cc -o $@

testobj/gtest.a : testobj/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^ -o $@

testobj/gtest_main.a : testobj/gtest-all.o testobj/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^ -o $@

testobj/test.o: $(TESTSRC)/test.cpp $(GTEST_HEADERS) $(TESTSRCs)
	g++ $(CPPFLAGS) $(INC) $(CXXFLAGS) -std=c++17 -c $(TESTSRC)/test.cpp -o $@

testobj/Board.o: $(TESTSRC)/Board.cpp
	$(CXX) $(INC) -c $^ -o $@

testobj/Game.o: $(TESTSRC)/Game.cpp
	$(CXX) $(INC) -c $^ -o $@

testobj/GameManager.o: $(TESTSRC)/GameManager.cpp
	$(CXX) $(INC) -c $^ -o $@

testobj/Position.o: $(TESTSRC)/Position.cpp
	$(CXX) $(INC) -c $^ -o $@

mktest: testobj/test.o testobj/gtest_main.a $(TESTOBJs)
	g++ $(CPPFLAGS) $(CXXFLAGS) -std=c++17 $^ -o test

.PHONY: all directory testdir clean cleantest
