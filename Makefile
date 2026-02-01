CXX = g++
CXXFLAGS = -O2 -Wall -std=c++23 -pipe
MAKEFLAGS += --no-print-directory

SRC_DIR = src
SRC_DIRS = task task_manager file_utils request_manager
CXXFLAGS += $(addprefix -I../$(SRC_DIR)/,$(SRC_DIRS))

LIBS_DIR = libs
LIBS_DIRS = cpp-httplib
CXXFLAGS += $(addprefix -I../$(LIBS_DIR)/,$(LIBS_DIRS))

TARGET = ctodo

OUT_DIR = output
BUILD_DIR = build

.PHONY: objects object build clean all client

all: clean objects build

objects:
	mkdir -p $(BUILD_DIR)
	@for dir in $(SRC_DIRS); do \
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
	$(CXX) -o $(OUT_DIR)/$(TARGET)-server $(BUILD_DIR)/*.o

client:
	source venv/bin/activate; pyinstaller client/python/client.py -F --distpath $(OUT_DIR) -n $(TARGET)

clean:
	rm -rf $(OUT_DIR)
	rm -rf $(BUILD_DIR)
