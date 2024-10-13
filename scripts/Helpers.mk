# Makefile Helpers
# --------------------------------------------------------------------------------

dyn-var = $(eval $(1) = $(2))
set-var = $(eval $(1) := $(2))
escape-str = "\"$(1)\""
definition = -D$(1)=$(call escape-str,$(2))

define clear_build
	$(call unset-var, BUILD_NAME)
	$(call unset-var, SRC_FILES)
endef

define clear_cxx_flags
	$(call unset-var, DEFINES)
	$(call unset-var, INCLUDES)
	$(call unset-var, FLAGS)
	$(call unset-var, LIBS)
endef

define compile_flags
	@echo $(CLANGD_FLAGS) $(CLANGD_INCLUDES) $(CLANGD_LIBS) $(CLANGD_DEFINES) | sed -e 's/"[^"]*"/""/g' -e 's/c++23/c++2b/g' -e 's/ /\n/g' | sort | uniq > ./compile_flags.txt
endef

define clear_clangd
	$(call unset-var, CLANGD_LIBS)
	$(call unset-var, CLANGD_DEFINES)
	$(call unset-var, CLANGD_FLAGS)
	$(call unset-var, CLANGD_INCLUDES)
endef

# --------------------------------------------------------------------------------
