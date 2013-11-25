
help:
	@echo "	Available targets:"
	@echo "	all"
	@echo "		- compile all files"
	@echo "	flash"
	@echo "		- flash binary to chip with st-flash"
	@echo "	cleanDebug"
	@echo "		- clean all"
	@echo "	doc"
	@echo "		- compile documentation with doxygen"
	@echo "	createtask TASK_NAME=name"
	@echo "		- create a new task for project"

.PHONY: clean cleanDebug doc help 


