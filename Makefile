
MODULES = ccsds pus rtos smpc time utils

# Color definitions
# TODO disable these if the console is not able to output color
CINFO  = \033[;0;33m
COK    = \033[32;01m
CERROR = \033[31;01m
CEND   = \033[0m

default: test

doc: design doxygen

design:
	@for m in $(MODULES); do \
		printf "$(CINFO)Build design description for module \"$$m\":$(CEND)\n" ; \
		make -C modules/$$m design --no-print-directory || return 1 ; \
	done

doxygen:
	@for m in $(MODULES); do \
		printf "$(CINFO)Build doxygen documentation for module \"$$m\":$(CEND)\n" ; \
		make -C modules/$$m doxygen --no-print-directory || return 1 ; \
	done

test:
	@for m in $(MODULES); do \
		printf "$(CINFO)Run unit tests for module \"$$m\":$(CEND)\n" ; \
		make -C modules/$$m test --no-print-directory || return 1 ; \
	done
	@printf "\n$(COK)[PASS] All unit tests passed!$(CEND)\n"

style:
	@for m in $(MODULES); do \
		printf "$(CINFO)Check style for module \"$$m\":$(CEND)\n" ; \
		make -C modules/$$m style --no-print-directory ; \
	done

clean :
	@for m in $(MODULES); do \
		printf "$(CINFO)Clean module \"$$m\":$(CEND)\n" ; \
		make -C modules/$$m clean --no-print-directory ; \
	done

.PHONY: doc

