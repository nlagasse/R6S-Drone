# compiler
CXX := g++

# compiler flags
CXXFLAGS := `sdl2-config --libs --cflags` -ggdb3 -O0 -Wall -lSDL2_image

# source files
SRCS := main.cpp clientNet.cpp

# object files
OBJS := $(SRCS:.cpp=.o)

# executable
EXEC := main

all: $(EXEC)
 
$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CXX) -o $@ $(OBJS) $(CXXFLAGS)

clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean