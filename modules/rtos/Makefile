# Copyright (c) 2013, German Aerospace Center (DLR)
# All Rights Reserved.

MODULE=rtos

LCOV_REMOVE_PATTERN = \
	"utils/*" "time/*"

# Slash at the end is important!
LIBCOBC_HW_PATH = /home/user/development/ry-avs-outpost/lib/sw/outpost-hw/

all: test

include ../module.default.mk

test: test-default

coverage: coverage-default

update-integration:
	mkdir -p ext/outpost-hw-backup/src
	mkdir -p ext/outpost-hw/
	rsync -a --delete -b --backup-dir=ext/outpost-hw-backup/ $(LIBCOBC_HW_PATH) ext/outpost-hw/

test-integration: update-integration
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

clean: clean-default

distclean: distclean-default
	$(RM) -r ext/outpost-hw

.PHONY: test coverage coverage-view clean

