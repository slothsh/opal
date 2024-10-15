# Target: common
# --------------------------------------------------------------------------------

define clear_opal_common
	$(call clear_cxx_flags)
	$(call clear_build)
	$(call set-var, VERSION)
endef

define prepare_opal_common
	$(call clear_opal_common)
	$(call set-var, BUILD_NAME, opal-common)
	$(call set-var, SRC_FILES, $(wildcard $(SRC_DIR)/*.cpp))
	$(call set-var, OBJS, $(patsubst $(SRC_DIR)/%.o,$(BUILD_DIR)/obj/$(BUILD_NAME)/%.o,$(patsubst %.cpp,%.o,$(SRC_FILES))))
	$(call set-var, FLAGS, -std=c++23 -Wall -Wpedantic -Wextra -O2)
	$(call set-var, INCLUDES, -I$(SRC_DIR) -I$(BUILD_PROTOCOLS_DIR) $(shell pkg-config --cflags fmt))
	$(call set-var, LIBS, $(shell pkg-config --libs fmt wayland-server))
	$(call set-var, VERSION,$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_SUB))
	$(call set-var, DEFINES,										  \
		$(call definition,PROGRAM_NAME,$(BUILD_NAME))				  \
		$(call definition,PROGRAM_DESCRIPTION,$(PROJECT_DESCRIPTION)) \
		$(call definition,PROJECT_AUTHOR,$(PROJECT_AUTHOR))			  \
		$(call definition,VERSION,$(VERSION))						  \
		$(call definition,GIT_HASH,$(GIT_HASH))
	)
endef

.PHONY: opal-common
opal-common: opal-common-build

.PHONY: opal-common-build
opal-common-build: configure wlprotocols
	$(call prepare_opal_common)
	@mkdir -p $(BUILD_DIR)/obj/$(BUILD_NAME)
	$(MAKE) BUILD_NAME=$(BUILD_NAME) $(OBJS)
	$(call clear_opal_common)

$(BUILD_DIR)/obj/$(BUILD_NAME)/%.o: $(SRC_DIR)/%.cpp
	$(call prepare_opal_common)
	$(CXX) $(FLAGS) $(INCLUDES) $(DEFINES) $(LIBS) -c -o $@ $<
	$(call clear_opal_common)

.PHONY: clangd-flags-opal-common
clangd-flags-opal-common:
	$(call clear_clangd)
	$(call prepare_opal_common)
	$(call set-var, CLANGD_LIBS, $(LIBS))
	$(call set-var, CLANGD_FLAGS, $(FLAGS))
	$(call set-var, CLANGD_INCLUDES, $(INCLUDES))
	$(call set-var, CLANGD_DEFINES, $(DEFINES))
	$(call compile_flags)
	$(call clear_opal_common)
	$(call clear_clangd)

# --------------------------------------------------------------------------------


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
	$(call set-var, SRC_FILES, $(wildcard $(SRC_DIR)/shell/*.cpp))
	$(call set-var, OBJS, $(wildcard $(BUILD_DIR)/obj/opal-common/*.o $(BUILD_DIR)/obj/wlprotocols/*.o))
	$(call set-var, FLAGS, -std=c++23 -Wall -Wpedantic -Wextra -O2)
	$(call set-var, INCLUDES, -I$(SRC_DIR) -I$(SRC_DIR)/shell -I$(BUILD_PROTOCOLS_DIR) $(shell pkg-config --cflags fmt))
	$(call set-var, LIBS, $(shell pkg-config --libs fmt wayland-server))
	$(call set-var, VERSION,$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_SUB))
	$(call set-var, DEFINES,										  \
		$(call definition,PROGRAM_NAME,$(BUILD_NAME))				  \
		$(call definition,PROGRAM_DESCRIPTION,$(PROJECT_DESCRIPTION)) \
		$(call definition,PROJECT_AUTHOR,$(PROJECT_AUTHOR))		   	  \
		$(call definition,VERSION,$(VERSION))						  \
		$(call definition,GIT_HASH,$(GIT_HASH))
	)
endef

.PHONY: opal-shell
opal-shell: opal-common opal-shell-build

.PHONY: opal-shell-build
opal-shell-build: configure
	$(call prepare_opal_shell)
	@mkdir -p $(BUILD_TARGETS_DIR)/$(BUILD_NAME)
	$(CXX) $(FLAGS) $(INCLUDES) $(DEFINES) $(LIBS) -o $(BUILD_TARGETS_DIR)/$(BUILD_NAME)/$(BUILD_NAME) $(SRC_FILES) $(OBJS)
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


# Target: opal-taskbar
# --------------------------------------------------------------------------------

define clear_opal_taskbar
	$(call clear_cxx_flags)
	$(call clear_build)
	$(call set-var, VERSION)
endef

define prepare_opal_taskbar
	$(call clear_opal_taskbar)
	$(call set-var, BUILD_NAME, opal-taskbar)
	$(call set-var, SRC_FILES, $(wildcard $(SRC_DIR)/taskbar/*.cpp))
	$(call set-var, OBJS, $(wildcard $(BUILD_DIR)/obj/opal-common/*.o $(BUILD_DIR)/obj/wlprotocols/*.o))
	$(call set-var, FLAGS, -std=c++23 -Wall -Wpedantic -Wextra -O2)
	$(call set-var, INCLUDES, -I$(SRC_DIR) -I$(SRC_DIR)/taskbar -I$(BUILD_PROTOCOLS_DIR) $(shell pkg-config --cflags fmt))
	$(call set-var, LIBS, $(shell pkg-config --libs fmt wayland-client wayland-server wlroots-0.18 pixman-1 egl wayland-egl glesv2))
	$(call set-var, VERSION,$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_SUB))
	$(call set-var, DEFINES,										  \
		$(call definition,PROGRAM_NAME,$(BUILD_NAME))				  \
		$(call definition,PROGRAM_DESCRIPTION,$(PROJECT_DESCRIPTION)) \
		$(call definition,PROJECT_AUTHOR,$(PROJECT_AUTHOR))		   	  \
		$(call definition,VERSION,$(VERSION))						  \
		$(call definition,GIT_HASH,$(GIT_HASH))
	)
endef

.PHONY: opal-taskbar
opal-taskbar: opal-common opal-taskbar-build

.PHONY: opal-taskbar-build
opal-taskbar-build: configure wlprotocols
	$(call prepare_opal_taskbar)
	@mkdir -p $(BUILD_TARGETS_DIR)/$(BUILD_NAME)
	$(CXX) $(FLAGS) $(INCLUDES) $(DEFINES) $(LIBS) -o $(BUILD_TARGETS_DIR)/$(BUILD_NAME)/$(BUILD_NAME) $(SRC_FILES) $(OBJS)
	$(call clear_opal_taskbar)

.PHONY: clangd-flags-opal-taskbar
clangd-flags-opal-taskbar:
	$(call clear_clangd)
	$(call prepare_opal_taskbar)
	$(call set-var, CLANGD_LIBS, $(LIBS))
	$(call set-var, CLANGD_FLAGS, $(FLAGS))
	$(call set-var, CLANGD_INCLUDES, $(INCLUDES))
	$(call set-var, CLANGD_DEFINES, $(DEFINES))
	$(call compile_flags)
	$(call clear_opal_taskbar)
	$(call clear_clangd)

# --------------------------------------------------------------------------------
