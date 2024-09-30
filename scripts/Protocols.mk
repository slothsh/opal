# Target: wayland-protocols
# --------------------------------------------------------------------------------

define clear_wlprotocols
	$(call clear_cxx_flags)
	$(call clear_build)
endef

PROTOCOLS := $(wildcard $(PROTOCOLS_DIR)/*.xml)

.PHONY: wlprotocols
wlprotocols: configure wlprotocols-build
	$(call clear_wlprotocols)

.PHONY: wlprotocols-build
wlprotocols-build: $(PROTOCOLS)
	@$(WLSCANNER) $< $(BUILD_PROTOCOLS_DIR)

undefine PROTOCOLS
undefine C_HEADERS
undefine C_SRC

# --------------------------------------------------------------------------------
