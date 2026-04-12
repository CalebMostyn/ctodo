CXX = g++
CXXFLAGS = -O2 -Wall -std=c++23 -pipe -MMD -MP -flto
MAKEFLAGS += --no-print-directory -j$(nproc)

SRC_DIR = src
SRC_DIRS = . task task_manager request_manager utils
CXXFLAGS += $(addprefix -I$(SRC_DIR)/,$(SRC_DIRS))

LIBS_DIR = libs
LIBS_DIRS = cpp-httplib nlohmann
CXXFLAGS += $(addprefix -I$(LIBS_DIR)/,$(LIBS_DIRS))

TARGET = ctodo

OUT_DIR = output
BUILD_DIR = build

DESTDIR =
PREFIX = /usr

USER := $(if $(SUDO_USER),$(SUDO_USER),$(USER))

SRCS := $(foreach d,$(SRC_DIRS),$(wildcard $(SRC_DIR)/$(d)/*.cpp))
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

.PHONY: all client install uninstall clean

$(OUT_DIR)/$(TARGET)-server: $(OBJS)
	@mkdir -p $(OUT_DIR)
	$(CXX) $^ $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(OUT_DIR)/$(TARGET)-server client

# must be ran from within virtual env or with requirements installed globally
client:
	pyinstaller client/python/client.py --distpath $(OUT_DIR) -n $(TARGET)

install:
	install -Dm755 $(OUT_DIR)/$(TARGET)-server $(DESTDIR)$(PREFIX)/bin/$(TARGET)-server
	install -Dm755 package/usr/bin/$(TARGET) $(DESTDIR)$(PREFIX)/bin/$(TARGET)

	install -d $(DESTDIR)$(PREFIX)/opt/$(TARGET)
	cp -r $(OUT_DIR)/$(TARGET)/* $(DESTDIR)$(PREFIX)/opt/$(TARGET)

	install -Dm644 package/usr/share/$(TARGET)/default.conf $(DESTDIR)$(PREFIX)/share/$(TARGET)/default.conf

	install -Dm644 package/etc/systemd/system/$(TARGET).service $(DESTDIR)/etc/systemd/system/$(TARGET).service
	sed -i 's/User=/User=$(USER)/g' $(DESTDIR)/etc/systemd/system/$(TARGET).service
	sed -i 's/Group=/Group=$(USER)/g' $(DESTDIR)/etc/systemd/system/$(TARGET).service

uninstall:
	rm -rf $(DESTDIR)$(PREFIX)/bin/$(TARGET)*
	rm -rf $(DESTDIR)$(PREFIX)/opt/$(TARGET)*
	rm -rf $(DESTDIR)$(PREFIX)/share/$(TARGET)*
	rm -rf $(DESTDIR)/etc/systemd/system/$(TARGET)*

clean:
	rm -rf $(OUT_DIR)
	rm -rf $(BUILD_DIR)

-include $(DEPS)
