CXX = g++
CXXFLAGS = -O2 -Wall -std=c++23 -pipe -MMD -MP -flto
MAKEFLAGS += --no-print-directory -j$(nproc)

SRC_DIR = src
SRC_DIRS = . task task_manager request_manager utils
CXXFLAGS += $(addprefix -I$(SRC_DIR)/,$(SRC_DIRS))

LIBS_DIR = libs
LIBS_DIRS = cpp-httplib
CXXFLAGS += $(addprefix -I$(LIBS_DIR)/,$(LIBS_DIRS))

TARGET = ctodo

OUT_DIR = output
BUILD_DIR = build

SRCS := $(foreach d,$(SRC_DIRS),$(wildcard $(SRC_DIR)/$(d)/*.cpp))
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

.PHONY: all client clean

$(OUT_DIR)/$(TARGET)-server: $(OBJS)
	@mkdir -p $(OUT_DIR)
	$(CXX) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(OUT_DIR)/$(TARGET)-server client

client:
	. venv/bin/activate; pyinstaller client/python/client.py -F --distpath $(OUT_DIR) -n $(TARGET)

clean:
	rm -rf $(OUT_DIR)
	rm -rf $(BUILD_DIR)

-include $(DEPS)
