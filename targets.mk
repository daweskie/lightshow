

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
	if [ -z "$(TASK_NAME)" ];  then  \
	    echo "need set TASK_NAME=<name>"; exit 1; \
	 else  \
	    echo "create"; \
	   ../createtask $(TASK_NAME); \
	fi

.PHONY: clean cleanDebug doc


