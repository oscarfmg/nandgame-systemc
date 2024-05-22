SRC = $(wildcard *.cpp)

OBJ = $(patsubst %.cpp, %.o, $(SRC))

SYSTEMC_HOME := /scratch/ofmendoz/sc/systemc-2.3.3
FMT_HOME := /scratch/ofmendoz/sc/fmt

INCLUDES := -I${SYSTEMC_HOME}/include \
            -I${FMT_HOME}/include \
            -I/scratch/ofmendoz/sc/nandgame

CXXFLAGS := -std=c++11 ${INCLUDES}

LDFLAGS := -L${SYSTEMC_HOME}/lib-linux64 \
           -L${FMT_HOME}/lib64

LIBS :=    -lsystemc \
	   -lfmt \
	   -Wl,-rpath=${SYSTEMC_HOME}/lib-linux64 \
	   -Wl,-rpath=${FMT_HOME}/lib64

MODULE_NAME := $(notdir $(shell pwd))

prog: $(OBJ)
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LIBS) $(OBJ) -o $(MODULE_NAME)

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm *.o >& /dev/null ||:
	@rm $(MODULE_NAME) >& /dev/null ||:
	@rm *.vcd >& /dev/null ||:
	@rm *~ >& /dev/null ||:
	@rm core.* >& /dev/null ||:

all: prog
.PHONY: all
