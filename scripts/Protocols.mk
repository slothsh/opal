# Target: wayland-protocols
# --------------------------------------------------------------------------------

define clear_wlprotocols
	$(call clear_cxx_flags)
	$(call clear_build)
endef

define prepare_wlprotocols
	$(call clear_wlprotocols)
	$(call set-var, BUILD_NAME, wlprotocols)
	$(call set-var, SRC_FILES, $(wildcard $(BUILD_PROTOCOLS_DIR)/*.c))
	$(call set-var, OBJS, $(patsubst $(BUILD_PROTOCOLS_DIR)/%.o,$(BUILD_DIR)/obj/$(BUILD_NAME)/%.o,$(patsubst %.c,%.o,$(SRC_FILES))))
	$(call set-var, FLAGS, -std=c11 -Wall -Wpedantic -Wextra -O2)
	$(call set-var, INCLUDES, -I$(BUILD_PROTOCOLS_DIR))
endef

PROTOCOLS := $(wildcard $(PROTOCOLS_DIR)/*.xml)
CLIENT_HEADERS := $(patsubst $(PROTOCOLS_DIR)/%,$(BUILD_PROTOCOLS_DIR)/%,$(patsubst %.xml,%.h,$(PROTOCOLS)))
CLIENT_CODE := $(patsubst $(PROTOCOLS_DIR)/%,$(BUILD_PROTOCOLS_DIR)/%,$(patsubst %.xml,%.c,$(PROTOCOLS)))

.PHONY: wlprotocols
wlprotocols: configure $(CLIENT_HEADERS) $(CLIENT_CODE) wlprotocols-build
	@echo "done!"

.PHONY: wlprotocols-build
wlprotocols-build:
	$(call prepare_wlprotocols)
	@mkdir -p $(BUILD_DIR)/obj/$(BUILD_NAME)
	$(MAKE) BUILD_NAME=$(BUILD_NAME) $(OBJS)
	$(call clear_wlprotocols)

$(BUILD_PROTOCOLS_DIR)/%.h: $(PROTOCOLS_DIR)/%.xml
	@$(WLSCANNER) client-header $< $@

$(BUILD_PROTOCOLS_DIR)/%.c: $(PROTOCOLS_DIR)/%.xml
	@$(WLSCANNER) private-code $< $@

$(BUILD_DIR)/obj/$(BUILD_NAME)/%.o: $(BUILD_PROTOCOLS_DIR)/%.c
	$(call prepare_wlprotocols)
	$(CC) $(FLAGS) $(INCLUDES) -c -o $@ $<
	$(call clear_wlprotocols)

undefine PROTOCOLS
undefine CLIENT_HEADERS
undefine CLIENT_CODE

# --------------------------------------------------------------------------------
