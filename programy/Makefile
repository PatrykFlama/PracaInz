# Compiler
CXX = g++
CXXFLAGS = -Wall -g

TARGET = main
SRCS = main.cpp

.PHONY: all run clean

all: $(TARGET) run

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^

run:
	./$(TARGET)

clean:
	rm -f $(TARGET)
