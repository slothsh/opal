# Print Config Info
# --------------------------------------------------------------------------------

.PHONY: info-config
info-config: info-config-head info-config-print-project info-config-tail
	@echo -n ""

.PHONY: info-config-all
info-config-all: info-config-head info-config-print-makefile info-config-print-project info-config-tail
	@echo -n ""

.PHONY: info-config-head
info-config-head:
	@echo "Configuration"
	@echo "--------------------------------------------------"

.PHONY: info-config-print-makefile
info-config-print-makefile:
	$(foreach v, 												     			       \
		$(filter-out $(PRE_MAKEFILE_VARIABLES) PRE_MAKEFILE_VARIABLES, $(.VARIABLES)), \
		$(info $(v)=$($(v))))

.PHONY: info-config-print-project
info-config-print-project:
	$(foreach v, 												     			               \
		$(filter-out $(PRE_CONFIG_VARIABLES) PRE_CONFIG_VARIABLES, $(PRE_MAKEFILE_VARIABLES)), \
		$(info $(v)=$($(v))))

.PHONY: info-config-tail
info-config-tail:
	@echo "--------------------------------------------------"
	
# --------------------------------------------------------------------------------
