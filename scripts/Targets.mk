# Target: opal-shell
# --------------------------------------------------------------------------------

define clear_opal_shell
	$(call clear_cxx_flags)
	$(call clear_build)
	$(call set-var, VERSION)
endef

define prepare_opal_shell
	$(call clear_opal_shell)
	$(call set-var, BUILD_NAME, opal-shell)
	$(call set-var, SRC_FILES, $(wildcard $(SRC_DIR)/*.cpp))
	$(call set-var, FLAGS, -std=c++23 -Wall -Wpedantic -Wextra -O2)
	$(call set-var, INCLUDES, -I$(BUILD_PROTOCOLS_DIR) $(shell pkg-config --cflags fmt))
	$(call set-var, LIBS, $(shell pkg-config --libs fmt wayland-server))
	$(call set-var, VERSION,$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_SUB))
	$(call set-var, DEFINES,                                          \
		$(call definition,PROGRAM_NAME,$(BUILD_NAME))                 \
	  	$(call definition,PROGRAM_DESCRIPTION,$(PROJECT_DESCRIPTION)) \
	  	$(call definition,PROJECT_AUTHOR,$(PROJECT_AUTHOR))     	  \
	  	$(call definition,VERSION,$(VERSION))
	)
endef

.PHONY: opal-shell
opal-shell: opal-shell-build

.PHONY: opal-shell-build
opal-shell-build: configure
	$(call prepare_opal_shell)
	@mkdir -p $(BUILD_TARGETS_DIR)/$(BUILD_NAME)
	$(CXX) $(FLAGS) $(INCLUDES) $(DEFINES) $(LIBS) -o $(BUILD_TARGETS_DIR)/$(BUILD_NAME)/$(BUILD_NAME) $(SRC_FILES)
	$(call clear_opal_shell)

.PHONY: clangd-flags-opal-shell
clangd-flags-opal-shell:
	$(call clear_clangd)
	$(call prepare_opal_shell)
	$(call set-var, CLANGD_LIBS, $(LIBS))
	$(call set-var, CLANGD_FLAGS, $(FLAGS))
	$(call set-var, CLANGD_INCLUDES, $(INCLUDES))
	$(call set-var, CLANGD_DEFINES, $(DEFINES))
	$(call compile_flags)
	$(call clear_opal_shell)
	$(call clear_clangd)

# --------------------------------------------------------------------------------
