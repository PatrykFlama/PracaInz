# Compiler
CXX = g++
CXXFLAGS_STD = -std=c++20 -O2
CXXFLAGS_DEV = -Wall -Wextra -pedantic -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -fstack-protector
CXXFLAGS = $(CXXFLAGS_STD) $(CXXFLAGS_DEV)
CPPFLAGS = -MMD -MP

TARGET = main
MAIN = main.cpp
OBJ = $(MAIN:.cpp=.o)
DEPS = $(OBJ:.o=.d)
# Other .cpp files are included directly (no headers), so track them as dependencies
EXTRA_SRCS = $(shell find . -name '*.cpp' ! -name 'main.cpp')

.PHONY: all run clean remake release

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ): $(MAIN) $(EXTRA_SRCS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $(MAIN)

release: CXXFLAGS = $(CXXFLAGS_STD)
release: CPPFLAGS =
release: $(MAIN) $(EXTRA_SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $(MAIN)

run: $(TARGET)
	./$(TARGET)

remake: clean all

clean:
	rm -f $(TARGET) release $(OBJ) $(DEPS)

-include $(DEPS)
