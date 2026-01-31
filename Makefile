CXX = g++
CXXFLAGS = -O2 -Wall -std=c++23

TARGET = ctodo

OUT_DIR = output
BUILD_DIR = build
SRC_DIR = src

.PHONY: objects build clean

all: clean objects build

objects:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR); $(CXX) $(CXXFLAGS) -c ../$(SRC_DIR)/*.cpp

build: objects
	mkdir -p $(OUT_DIR)
	$(CXX) -o $(OUT_DIR)/$(TARGET) $(BUILD_DIR)/*.o

clean:
	rm -rf $(OUT_DIR)
	rm -rf $(BUILD_DIR)
