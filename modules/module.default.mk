# Copyright (c) 2013-2014, 2016-2018, German Aerospace Center (DLR)
#
# This file is part of the development version of OUTPOST.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Authors:
# - 2013-2014, 2016-2018, Fabian Greif (DLR RY-AVS)
# - 2016, Jan Sommer (DLR SC-SRV)
# - 2016, Rhea Rinaldo (DLR RY-AVS)

# Configure the parallel build normal "make -jn" is not possible as SCons
# is used to build the source files. And make has no option the extract the
# number of request parallel processes from the "-j" option.
ifneq ($(JOBS),)
  MAKEJOBS=-j$(JOBS)
else
  # Default is 3 parallel jobs. Use `JOBS=` to disable parallel build.
  NPROCS:=1
  OS:=$(shell uname -s)
  ifeq ($(OS),Linux)
    NPROCS:=$(shell nproc)
  endif
  MAKEJOBS=-j$(NPROCS)
endif

# Check if the terminal supports colors
COLORS := $(shell tput colors 2> /dev/null)

ifeq ($(COLORS),)
# The terminal doesn't support colors, don't output anything
CINFO  = 
COK    = 
CWARN  = 
CERROR = 
CEND   = 
else
# Color definitions (e.g. for bash 'tput colors' returns '8')
CINFO  = \033[;0;33m
COK    = \033[32;01m
CWARN  = \033[33;01m
CERROR = \033[31;01m
CEND   = \033[0m
endif

# Filter for the Unittest runners.
GTEST_FILTER ?= *

LCOV_DEFAULT_REMOVE_PATTERN = "/usr*" "test/*" "default/*" "tools/*" "utils-ext/*"

# Path relative from the module folders (e.g. '/trunk/modules/pus').
MODULEPATH_ABSOLUTE ?= $(shell readlink -f .)
ROOTPATH  ?= ../..
ROOTPATH_ABSOLUTE = $(shell readlink -f $(ROOTPATH))
BUILDPATH ?= $(ROOTPATH)/build
BUILDPATH_ABSOLUTE = $(shell readlink -f $(BUILDPATH))
POLYSPACE ?= $(ROOTPATH)/tools/polyspace

# Allow to specifiy a specific set of Polyspace rules to check.
ifneq ($(RULES),)
  POLYSPACE_RULES=--rules="$(RULES)"
else
  POLYSPACE_RULES=
endif

#help:
#	@echo -e " Please use \`make $(CINFO)<target>$(CEND)\` where $(CINFO)<target>$(CEND) is one of"
#	@echo -e "  $(CINFO)all$(CEND)                Compile and run unit tests"
#	@echo -e "  $(CINFO)test$(CEND)               Compile and run unit tests"
#	@echo -e "  $(CINFO)coverage$(CEND)           Generate coverage analysis"
#	@echo -e "  $(CINFO)coverage-view$(CEND)      View coverage analysis"
#	@echo -e "  $(CINFO)doxygen-view$(CEND)       View doxygen documentation"
#	@echo -e "  $(CINFO)clean$(CEND)              Clean module log"

build-test:
	@scons -C test/ -Q $(MAKEJOBS) $(MODULE)

test-trap:
	@scons -C ../ -Q $(MAKEJOBS) $(MODULE) trap=1
	@$(BUILDPATH)/test/modules/$(MODULE)/test/runner --gtest_filter=$(GTEST_FILTER) --gtest_output=xml:$(BUILDPATH)/test/modules/$(MODULE)/test/coverage.xml
	@mkdir -p $(BUILDPATH)/test
	@python3 $(ROOTPATH)/tools/gtest_process_skipped.py $(BUILDPATH)/test/modules/$(MODULE)/test/coverage.xml $(BUILDPATH)/test/$(MODULE).xml

test-default:	
	@scons -C ../ -Q $(MAKEJOBS) $(MODULE)
	@$(BUILDPATH)/test/modules/$(MODULE)/test/runner --gtest_filter=$(GTEST_FILTER) --gtest_output=xml:$(BUILDPATH)/test/modules/$(MODULE)/test/coverage.xml
	@mkdir -p $(BUILDPATH)/test
	@python3 $(ROOTPATH)/tools/gtest_process_skipped.py $(BUILDPATH)/test/modules/$(MODULE)/test/coverage.xml $(BUILDPATH)/test/$(MODULE).xml

test-verbose-default:
	@scons -C ../ -Q $(MAKEJOBS) build
	@$(BUILDPATH)/test/modules/$(MODULE)/test/runner --gtest_filter=$(GTEST_FILTER) --gtest_output=xml:$(BUILDPATH)/test/modules/$(MODULE)/test/coverage.xml --verbose
	@mkdir -p $(BUILDPATH)/test
	@python3 $(ROOTPATH)/tools/gtest_process_skipped.py $(BUILDPATH)/test/modules/$(MODULE)/test/coverage.xml $(BUILDPATH)/test/$(MODULE).xml
    
cppcheck:
	@scons -C ../ $(MODULE)_db -D append_buildpath=cppcheck
	@mkdir -p $(BUILDPATH)/cppcheck
	@cd ../ ;\
	for DB in `find $(BUILDPATH_ABSOLUTE)/cppcheck/$(MODULE) -iname "compile_commands.json"` ;\
	do \
		cppcheck $(MAKEJOBS) --quiet --language=c++ --library=googletest --enable=all --inconclusive --force --inline-suppr --project=$$DB --suppressions-list=$(ROOTPATH_ABSOLUTE)/.cppcheck-excludes --xml --xml-version=2 2> $(BUILDPATH_ABSOLUTE)/cppcheck/$(MODULE).xml;\
		cppcheck-htmlreport --file $(BUILDPATH_ABSOLUTE)/cppcheck/$(MODULE).xml --report-dir $(BUILDPATH_ABSOLUTE)/cppcheck/$(MODULE) --title="$(MODULE)";\
	done;

coverage-default:
	@scons $(MODULE) coverage=1 $(MAKEJOBS) -Q -C ../ || return 1; \
	find $(BUILDPATH)/coverage/modules/$(MODULE)/test/ -name "*.gcda" -delete; \
	$(BUILDPATH)/coverage/modules/$(MODULE)/test/runner; \
	cd $(BUILDPATH)/coverage/modules/$(MODULE)/; \
	gcovr --root=$(ROOTPATH_ABSOLUTE)/modules/ --filter=$(MODULEPATH_ABSOLUTE)/src/ --object-directory=. $(foreach dir,$(LCOV_REMOVE_PATTERN), -e $(dir)) --xml-pretty -o $(BUILDPATH_ABSOLUTE)/coverage/$(MODULE).xml; \
	sed -i 's+modules/$(MODULE)/test/++g' $(BUILDPATH_ABSOLUTE)/coverage/$(MODULE).xml; \
	sed -i 's+$(MODULEPATH_ABSOLUTE)/src/+modules/$(MODULE)/src/+g' $(BUILDPATH_ABSOLUTE)/coverage/$(MODULE).xml;

coverage-print-default: coverage-default
	@cd $(BUILDPATH)/coverage/modules/$(MODULE)/; \
	gcovr --root=$(ROOTPATH_ABSOLUTE)/modules/ --filter=$(MODULEPATH_ABSOLUTE)/src/ --object-directory=. $(foreach dir,$(LCOV_REMOVE_PATTERN), -e $(dir)) -s; 

coverage-html-default: coverage-default
	@cd $(BUILDPATH_ABSOLUTE)/coverage/modules/$(MODULE)/; \
	rm -rf report; \
	mkdir -p report; \
	gcovr --root=$(ROOTPATH_ABSOLUTE)/modules/ --filter=$(MODULEPATH_ABSOLUTE)/src/ --object-directory=. $(foreach dir,$(LCOV_REMOVE_PATTERN), -e $(dir)) --html-details report/index.html;

coverage-html-lcov: coverage-default
	@cd $(BUILDPATH)/coverage/modules/$(MODULE)/test; \
	lcov -t '$(MODULE) library' -o coverage.info -c --base-directory $(CURDIR)/test --directory . --gcov-tool `which gcov` || return 1; \
	lcov -r coverage.info $(LCOV_DEFAULT_REMOVE_PATTERN) $(LCOV_REMOVE_PATTERN) -o coverage.info; \
	rm -rf report; \
	genhtml --demangle-cpp -o report coverage.info

coverage-view-default:
	@xdg-open $(BUILDPATH)/coverage/modules/$(MODULE)/report/index.html &

cloc-source-default:
	@mkdir -p $(BUILDPATH)/cloc
	@cloc --by-file --xml --out=$(BUILDPATH)/cloc/$(MODULE).xml src/

analyze-clang:
	@scons -C ../ $(MODULE)_db -D append_buildpath=clang-tidy
	@cd $(BUILDPATH)/clang-tidy/$(MODULE)/; \
	run-clang-tidy $(MAKEJOBS)

codingstyle: codingstyle-simple codingstyle-jsf

# Run style checker
codingstyle-simple:
	@find src/ -regex ".*\.\(h\|cpp\)" | vera++ -p outpost --show-rule --summary --root ../../tools/vera++

codingstyle-jsf:
	@$(POLYSPACE)/QuickPS15.py analyse "src" \
	                         --config "$(POLYSPACE)/profiles/$(MODULE)/jsf-analysis/options.cfg" \
	                         --options "$(POLYSPACE)/profiles/$(MODULE)/jsf-analysis/options.txt" \
	                         --ssh "$(POLYSPACE)/ssh_info.ini" \
	                         -n -a -x

codingstyle-jsf-legacy:
	@$(POLYSPACE)/legacy/python/QuickPS.py analyse "src" \
	                         --config "$(POLYSPACE)/legacy/python/profiles/$(MODULE)/options_py.cfg" \
	                         --options "$(POLYSPACE)/legacy/python/profiles/$(MODULE)/options.txt" \
	                         --include "$(POLYSPACE)/polyspace_header.h" \
	                         --ssh "$(POLYSPACE)/ssh_info.ini" \
	                         -a -x

codingstyle-jsf-view:
	@xdg-open $(POLYSPACE)/profiles/$(MODULE)/results/jsf/Polyspace_R2015b_latest.log &

codingstyle-jsf-annotate:
	@tar xfz $(POLYSPACE)/profiles/$(MODULE)/results/jsf/polyspace_results.tgz -C $(POLYSPACE)/profiles/$(MODULE)/results/jsf
	@$(POLYSPACE)/polyspace_jsf_annotate.py -d "src/" \
	                                        -c "$(POLYSPACE)/profiles/$(MODULE)/jsf-analysis/options.cfg" \
	                                        -r "$(POLYSPACE)/profiles/$(MODULE)/results/jsf/Polyspace-Doc/$(MODULE)_Developer/root.html" \
	                                        $(POLYSPACE_RULES) \
	                                        --annotate 

codingstyle-jsf-remove-annotations:
	@$(POLYSPACE)/polyspace_jsf_annotate.py -d "src/" \
	                                        -c "$(POLYSPACE)/profiles/$(MODULE)/jsf-analysis/options.cfg" \
	                                        -r "$(POLYSPACE)/profiles/$(MODULE)/results/jsf/Polyspace-Doc/$(MODULE)_Developer/root.html"

doxygen:
	@doxygen doc/doxygen/doxyfile

doxygen-internal:
	@doxygen doc/doxygen/doxyfile-internal

design:
	@$(MAKE) -C doc/design

doc: doxygen design doxyge-internal

CLANG_FORMAT="clang-format"

FORMAT_SOURCE_FILES ?= $(shell find src/ test/ -type f -name '*.cpp')
FORMAT_HEADER_FILES ?= $(shell find src/ test/ -type f -name '*.h')

format:
	@$(CLANG_FORMAT) -i --style=file $(FORMAT_SOURCE_FILES)
	@$(CLANG_FORMAT) -i --style=file $(FORMAT_HEADER_FILES)

clean-default:
	@$(RM) -r doc/doxygen/api/*
	@$(RM) -r doc/doxygen/internal/*
	@$(RM) -r $(BUILDPATH)/test/$(MODULE)
	@$(RM) -r $(BUILDPATH)/coverage/$(MODULE)
	@$(RM) -r $(BUILDPATH)/test/$(MODULE).xml
	@$(RM) -r $(BUILDPATH)/cloc/$(MODULE).xml
	@$(RM) -r $(BUILDPATH)/coverage/$(MODULE).xml

distclean-default:
	@scons build coverage=1 -Q -C test/unit -c

.PHONY: test-default coverage-default coverage-print-default coverage-html-default coverage-html-lcov coverage-view-default cppcheck cppcheck-tests cppcheck-unittests
