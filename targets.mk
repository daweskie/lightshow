

Debug:all

flash: all
	st-flash write build/$(PROJECT).bin 0x8000000
	@echo "press reset"
	@read
	@sleep 2
	gtkterm -c disc &

cleanDebug: clean all

doc: 
	$(MAKE) -C doc

createtask:
	@if [ -z ${TASK_NAME} ]; \
	    then \
	    $(error need set TASK_NAME=name") \
	fi
	../createtask $(TASK_NAME)

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
	@echo "	createtask TASK_NAME=name"
	@echo "		- create a new task for project"

.PHONY: clean cleanDebug doc global_help 


