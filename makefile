CXX=g++
CXX_FLAGS=-Wall -Wextra -g
SRCS=$(wildcard *.cc)

.PHONY: build clean

all: build

build: $(SRCS)
	$(CXX) $(CXX_FLAGS) -o alg $(SRCS)

threaded: $(SRCS)
	$(CXX) $(CXX_FLAGS) -pthread -DTHREAD -o alg $(SRCS)

clean:
	rm -rf alg