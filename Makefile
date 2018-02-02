# Copyright (c) 2013-2017, German Aerospace Center (DLR)
#
# This file is part of the development version of OUTPOST.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Authors:
# - 2013-2017, Fabian Greif (DLR RY-AVS)
# - 2014, Murat Goeksu (DLR RY-AVS)

MODULES_GLOBAL = utils time smpc rtos hal comm
MODULES_TEST = l3test

MODULES_JSF = $(MODULES_GLOBAL) rtos

MODULES = $(MODULES_GLOBAL) $(MODULES_HW) $(MODULES_TEST)

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
COK    = \033[32;01m
CERROR = \033[31;01m
CEND   = \033[0m
endif

help:
	@echo "Please use \`make <target>\` where <target> is one of"
	@echo "  doc       to build all documentation"
	@echo "  design    to build the design documentation (requires xelatex)"
	@echo "  doxygen   to build the doxygen documentation"
	@echo "  test      to run all unit tests"
	@echo "  test-full to run all unit and compilation tests"
	@echo "  style     to check the coding style with vera++"
	@echo "  clean     to remove temporary data (\`build\` folder)"

doc: design doxygen

documents: design

design:
	@for m in $(MODULES); do \
		printf "\n$(CINFO)Build design description for module \"$$m\":$(CEND)\n" ; \
		make -C modules/$$m design --no-print-directory || return 1 ; \
	done

doxygen:
	@for m in $(MODULES); do \
		printf "\n$(CINFO)Build doxygen documentation for module \"$$m\":$(CEND)\n" ; \
		make -C modules/$$m doxygen --no-print-directory || return 1 ; \
	done

test:
	@for m in $(MODULES_GLOBAL) $(MODULES_TEST); do \
		printf "\n$(CINFO)Run unit tests for module \"$$m\":$(CEND)\n" ; \
		make -C modules/$$m test --no-print-directory || return 1 ; \
	done
	@printf "\n$(COK)[PASS] All unit tests passed!$(CEND)\n"

cloc-source:
	@for m in $(MODULES_GLOBAL) $(MODULES_TEST); do \
		printf "\n$(CINFO)Count Lines of Code for module \"$$m\":$(CEND)\n" ; \
		make -C modules/$$m cloc-source-default --no-print-directory || return 1 ; \
	done
	@printf "\n$(COK)[PASS] All unit tests passed!$(CEND)\n"

test-full: test
	echo "\n$(CINFO)Run compilation tests:$(CEND)\n"
	$(MAKE) -C test/sparc_rtems --no-print-directory
	@printf "\n$(COK)[PASS] All tests passed!$(CEND)\n"

coverage:
	@for m in $(MODULES_GLOBAL) $(MODULES_TEST); do \
		printf "\n$(CINFO)Run coverage analysis for module \"$$m\":$(CEND)\n" ; \
		make -C modules/$$m coverage --no-print-directory || return 1 ; \
	done
	@printf "\n$(COK)[PASS] Coverage analysis done!$(CEND)\n"

analyze-clang:
	@for m in $(MODULES); do \
		printf "\n$(CINFO)Run static analysis with clang for module \"$$m\":$(CEND)\n" ; \
		make -C modules/$$m analyze-clang --no-print-directory || return 1 ; \
	done
	@printf "\n$(COK)[PASS] Static analysis with clang done!$(CEND)\n"

codingstyle-simple:
	@for m in $(MODULES); do \
		printf "\n$(CINFO)Check style for module \"$$m\":$(CEND)\n" ; \
		make -C modules/$$m codingstyle-simple --no-print-directory ; \
	done

codingstyle-jsf:
	@for m in $(MODULES_JSF); do \
		printf "\n$(CINFO)Check style for module \"$$m\":$(CEND)\n" ; \
		make -C modules/$$m codingstyle-jsf --no-print-directory ; \
	done

codingstyle: codingstyle-simple codingstyle-jsf

metric:
	@for m in $(MODULES); do \
		printf "\n$(CINFO)Generating code metrics for module \"$$m\" (sources):$(CEND)\n" ; \
		sloccount --duplicates --wide modules/$$m/src ; \
		printf "\n$(CINFO)Generating code metrics for module \"$$m\" (unittests):$(CEND)\n" ; \
		sloccount --duplicates --wide modules/$$m/test ; \
	done

CLANG_FORMAT_CONTAINER=docker run --rm -it -v "$(PWD):/build" \
	-e BUILDER_UID=$(shell id -u) -e BUILDER_GID=$(shell id -g) \
	-e BUILDER_USER=$(shell id -un) -e BUILDER_GROUP=$(shell id -gn) \
	dlravs/clang-format

CLANG_FORMAT=$(CLANG_FORMAT_CONTAINER) "/usr/bin/clang-format-7"

SOURCE_FILES = $(shell find modules/*/src/ modules/*/test/ -type f -name '*.cpp')
HEADER_FILES = $(shell find modules/*/src/ modules/*/test/ -type f -name '*.h')

format:
	@$(CLANG_FORMAT) -i $(SOURCE_FILES)
	@$(CLANG_FORMAT) -i $(HEADER_FILES)

clean :
	@for m in $(MODULES); do \
		printf "\n$(CINFO)Clean module \"$$m\":$(CEND)\n" ; \
		make -C modules/$$m clean --no-print-directory ; \
	done

.PHONY: doc test clean

