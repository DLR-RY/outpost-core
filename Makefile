
MODULES = ccsds pus rtos smpc time utils

doc: design doxygen

design:
	@for m in $(MODULES); do \
		echo "Build design description for module \"$$m\":" ; \
		make -C modules/$$m design --no-print-directory || return 1 ; \
	done

doxygen:
	@for m in $(MODULES); do \
		echo "Build doxygen documentation for module \"$$m\":" ; \
		make -C modules/$$m doxygen --no-print-directory || return 1 ; \
	done

test:
	@for m in $(MODULES); do \
		echo "Run unit tests for module \"$$m\":" ; \
		make -C modules/$$m test --no-print-directory || return 1 ; \
	done

style:
	@for m in $(MODULES); do \
		echo "Check style for module \"$$m\":" ; \
		make -C modules/$$m style --no-print-directory ; \
	done

clean :
	@for m in $(MODULES); do \
		echo "Clean module \"$$m\":" ; \
		make -C modules/$$m clean --no-print-directory ; \
	done

.PHONY: doc

