
global_help:
	@echo "	Available targets:"
	@echo "	all"
	@echo "		- compile all files"
	@echo "	flash"
	@echo "		- flash binary to chip with st-flash"
	@echo "	cleanDebug"
	@echo "		- clean all"
	@echo "	doc"
	@echo "		- compile documentation with doxygen"

.PHONY: clean cleanDebug doc global_help 


