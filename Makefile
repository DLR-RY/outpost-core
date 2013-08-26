
MODULES = ccsds pus rtos smpc time utils

doc: design doxygen

design:
	@for m in $(MODULES); do \
		echo "Build design description for module \"$$m\":" ; \
		make -C modules/$$m design ; \
	done

doxygen:
	@for m in $(MODULES); do \
		echo "Build doxygen documentation for module \"$$m\":" ; \
		make -C modules/$$m doxygen ; \
	done

clean :
	@for m in $(MODULES); do \
		echo "Clean module \"$$m\":" ; \
		make -C modules/$$m clean ; \
	done

.PHONY: doc

