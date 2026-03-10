CXX=g++
CXX_FLAGS=-Wall -Wextra -g
SRCS=$(wildcard *.cc)

.PHONY: clean

build: $(SRCS)
	$(CXX) $(CXX_FLAGS) -o alg $(SRCS)

clean:
	rm -rf alg