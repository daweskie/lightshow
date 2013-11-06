

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

.PHONY: clean cleanDebug doc


