CXX=g++
CXX_FLAGS=-Wall -Wextra -O3 -march=native -mtune=native -std=c++11
SRCS=$(wildcard *.cc)

.PHONY: all clean

all: build

build: $(SRCS)
	$(CXX) $(CXX_FLAGS) -o alg $(SRCS)

random: $(SRCS)
	$(CXX) $(CXX_FLAGS) -o alg_random $(SRCS) -DRANDOM

clean:
	rm -rf alg alg_random
