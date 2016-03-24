
# Check if the terminal supports colors
COLORS := $(shell tput colors 2> /dev/null)

ifeq ($(COLORS),)
# The terminal doesn't support colors, don't output anything
CINFO  = 
COK    = 
CERROR = 
CEND   = 
else
# Color definitions (e.g. for bash 'tput colors' returns '8')
CINFO  = \033[;0;33m
COK    = \033[32m
CERROR = \033[31;01m
CEND   = \033[0m
endif

# Slash at the end is important!
LIBCOBC_HW_PATH = /home/user/development/ry-avs-cobc/lib/sw/libcobc-hw/

all: test

MODULE=rtos

include ../../conf/module.default.mk

test: update
	@printf "$(CINFO) Running RTOS abstraction layer compilation tests$(CEND)\n"
	
	@printf "$(CINFO) FreeRTOS (STM32F4 Discovery):$(CEND)\n"
	@scons $(MAKEJOBS) -Q -C it/freertos || return 1
	
	@printf "$(CINFO) None (OpenRisc 32):$(CEND)\n"
	@scons $(MAKEJOBS) -Q -C it/none_or32 || return 1
	
	@printf "$(CINFO) POSIX (Linux):$(CEND)\n"
	@scons $(MAKEJOBS) -Q -C it/posix || return 1
	
	@printf "$(CINFO) RTEMS (LEON3):$(CEND)\n"
	@scons $(MAKEJOBS) -Q -C it/rtems || return 1
	
	@printf "$(COK) RTOS abstraction layer compilation tests passed!$(CEND)\n"

coverage:
	@printf "$(CERROR)Not supported for this module!$(CEND)\n"

coverage-view:
	@printf "$(CERROR)Not supported for this module!$(CEND)\n"

clean: clean_default
	@$(RM) -r ../../build/$(MODULE)/*

update:
	mkdir -p ext/libcobc-hw-backup/src
	mkdir -p ext/libcobc-hw/
	rsync -a --delete -b --backup-dir=ext/libcobc-hw-backup/ $(LIBCOBC_HW_PATH) ext/libcobc-hw/

distclean: distclean_default
	$(RM) -r ext/libcobc-hw

.PHONY: test coverage coverage-view clean

