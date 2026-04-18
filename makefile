CXX=g++
CXX_FLAGS=-Wall -Wextra -O3 -march=native -mtune=native -std=c++11
SRCS=$(wildcard *.cc)

.PHONY: build clean

all: build

build: $(SRCS)
	$(CXX) $(CXX_FLAGS) -o alg $(SRCS)

threads: $(SRCS)
	$(CXX) $(CXX_FLAGS) -DTHREADS -o alg $(SRCS)

clean:
	rm -rf alg
