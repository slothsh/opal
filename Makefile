# Load Project Configuration
# --------------------------------------------------------------------------------

CONFIG := project.conf
PRE_CONFIG_VARIABLES := $(.VARIABLES)
include ${CONFIG}
PRE_MAKEFILE_VARIABLES := $(.VARIABLES)

# --------------------------------------------------------------------------------


# Paths
# --------------------------------------------------------------------------------

DATA_DIR = /usr/share
SYSTEM_INCLUDE_DIR = /usr/include

ROOT_DIR := $(abspath .)
SRC_DIR := $(abspath ./src)
SCRIPTS_DIR := $(abspath ./scripts)
PROTOCOLS_DIR := $(abspath ./protocols)

BUILD_DIR := $(abspath ./build)
BUILD_PROTOCOLS_DIR := $(abspath $(BUILD_DIR)/protocols)
BUILD_TARGETS_DIR := $(abspath $(BUILD_DIR)/targets)

# --------------------------------------------------------------------------------

# Makefile Headers
# --------------------------------------------------------------------------------

include $(SCRIPTS_DIR)/Protocols.mk
include $(SCRIPTS_DIR)/Helpers.mk
include $(SCRIPTS_DIR)/ConfigInfo.mk
include $(SCRIPTS_DIR)/Targets.mk

# --------------------------------------------------------------------------------


# Executables
# --------------------------------------------------------------------------------

CC := cc
CXX := c++
WLSCANNER := wayland-scanner

# --------------------------------------------------------------------------------


# Recipes
# --------------------------------------------------------------------------------

.PHONY: configure
configure:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_PROTOCOLS_DIR)

.PHONY: build
build: opal-shell

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)

# --------------------------------------------------------------------------------
