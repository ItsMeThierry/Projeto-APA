CXX = g++
CXXFLAGS = -Wall -Wextra -Isrc/include

SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = src/include

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

TARGET = $(BUILD_DIR)/main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run