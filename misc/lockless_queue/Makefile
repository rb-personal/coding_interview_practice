CXX=g++
SRC=$(wildcard *.cc)
HDR=$(wildcard *.hh)
OBJ=$(patsubst %.cc, %.o, $(wildcard *.cc))
EXE=lockless_queue.exe
CFLAGS=-std=c++0x -march=native -O3
LFLAGS=-pthread

all: $(SRC) $(EXE)

$(EXE): $(OBJ)
	$(CXX) $(LFLAGS) $(OBJ) -o $@

%.o: %.cc
	$(CXX) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -rf $(OBJ) $(EXE)
