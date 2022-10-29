C_SOURCES += $(shell find -L $(DEVICE_DIR_NAME)/Src -name \*.c)
CFLAGS += -I$(DEVICE_DIR_NAME)/Inc
