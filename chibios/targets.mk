

Debug:all

flash: all killterm
	st-flash write build/$(PROJECT).bin 0x8000000
	@echo "press reset"
	@read dummy
	@sleep 2
	../startterm

cleanDebug: clean all

doc: 
	$(MAKE) -C doc

createtask:
	echo -n "taskname:"; \
	read TASK_NAME ;     \
	echo taskname:$$TASK_NAME; \
	if [ -z "$$TASK_NAME" ];  then  \
	    echo "need set name of task"; exit 1; \
	else  \
	    echo "create"; \
	   ../createtask $$TASK_NAME; \
	fi

killterm:
	../killterm

.PHONY: clean cleanDebug doc killterm



