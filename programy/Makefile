# Compiler
CXX = g++
CXXFLAGS_STD = -std=c++20 -O2
CXXFLAGS_DEV = -Wall -Wextra -pedantic -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -fstack-protector
CXXFLAGS = $(CXXFLAGS_STD) $(CXXFLAGS_DEV)

TARGET = main
SRCS = main.cpp

.PHONY: all run clean

all: $(TARGET) run

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^


release:
	$(CXX) $(CXXFLAGS_STD) -o release $(SRCS)

run:
	./$(TARGET)

clean:
	rm -f $(TARGET) release
