CXX = g++
CXXFLAGS = -O2 -Wall -std=c++23 -pipe

DIRS = task task_manager file_utils
CXXFLAGS += $(addprefix -I../src/,$(DIRS))
MAKEFLAGS += --no-print-directory

TARGET = ctodo

OUT_DIR = output
BUILD_DIR = build
SRC_DIR = src

.PHONY: objects object build clean all

all: clean objects build

objects:
	mkdir -p $(BUILD_DIR)
	@for dir in $(DIRS); do \
		$(MAKE) object DIR=$$dir; \
	done
	@$(MAKE) object # src directory

object:
ifdef DIR
	cd $(BUILD_DIR); $(CXX) $(CXXFLAGS) -c ../$(SRC_DIR)/$(DIR)/*.cpp
else
	cd $(BUILD_DIR); $(CXX) $(CXXFLAGS) -c ../$(SRC_DIR)/*.cpp
endif

build: objects
	mkdir -p $(OUT_DIR)
	$(CXX) -o $(OUT_DIR)/$(TARGET) $(BUILD_DIR)/*.o

clean:
	rm -rf $(OUT_DIR)
	rm -rf $(BUILD_DIR)
