CSRCS += lv_port_indev.c
CSRCS += lv_port_disp.c

DEPPATH += --dep-path $(LVGL_DIR)/$(LV_DRIVERS_DIR_NAME)
VPATH += :$(LVGL_DIR)/$(LV_DRIVERS_DIR_NAME)

CFLAGS += "-I$(LVGL_DIR)/$(LV_DRIVERS_DIR_NAME)"
