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
ROOTPATH  ?= ../..
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

build-lua-default:
	@$(MAKE) --no-print-directory -C ../l3test build-lua

build-test:
	@scons -C test/ -Q $(MAKEJOBS) build
	
test-default:
	@scons -C test/ -Q $(MAKEJOBS) build
	@$(BUILDPATH)/$(MODULE)/test/unittest/runner --gtest_filter=$(GTEST_FILTER) --gtest_output=xml:$(BUILDPATH)/$(MODULE)/test/unittest/coverage.xml
	@mkdir -p $(BUILDPATH)/test
	@python3 $(ROOTPATH)/tools/gtest_process_skipped.py $(BUILDPATH)/$(MODULE)/test/unittest/coverage.xml $(BUILDPATH)/test/$(MODULE).xml

cppcheck:
	@scons -C test/ compiledb -D append_buildpath=compiledb
	@mkdir -p $(BUILDPATH_ABSOLUTE)/cppcheck;
	@for DB in `find $(BUILDPATH_ABSOLUTE)/$(MODULE) -iname "compile_db.json"` ;\
	do \
		cppcheck --enable=all --inconclusive --force --project=$$DB --xml --xml-version=2 2> $(BUILDPATH)/cppcheck/$(MODULE).xml;\
		cppcheck-htmlreport --file $(BUILDPATH)/cppcheck/$(MODULE).xml --report-dir $(BUILDPATH)/cppcheck/$(MODULE) --title="$(MODULE)";\
	done;


cppcheck-tests:
	@scons -C test/ compiledb_test -D append_buildpath=compiledb
	@mkdir -p $(BUILDPATH_ABSOLUTE)/cppcheck;
	@for DB in `find $(BUILDPATH_ABSOLUTE)/$(MODULE) -iname "compile_db_test.json"` ;\
	do \
		cppcheck --enable=all --inconclusive --force --project=$$DB --xml --xml-version=2 2> $(BUILDPATH)/cppcheck/$(MODULE)-test.xml;\
		cppcheck-htmlreport --file $(BUILDPATH)/cppcheck/$(MODULE)-test.xml --report-dir $(BUILDPATH)/cppcheck/$(MODULE)-test --title="$(MODULE)-test";\
	done;

cppcheck-unittests:
	@scons -C test/ compiledb_unittest -D append_buildpath=compiledb
	@mkdir -p $(BUILDPATH_ABSOLUTE)/cppcheck;
	@for DB in `find $(BUILDPATH_ABSOLUTE)/$(MODULE) -iname "compile_db_unittest.json"` ;\
	do \
		cppcheck --enable=all --inconclusive --force --project=$$DB --xml --xml-version=2 2> $(BUILDPATH)/cppcheck/$(MODULE)-unittest.xml;\
		cppcheck-htmlreport --file $(BUILDPATH)/cppcheck/$(MODULE)-unittest.xml --report-dir $(BUILDPATH)/cppcheck/$(MODULE)-unittest --title="$(MODULE)-unittest";\
	done;


coverage-default:
	@scons build coverage=1 $(MAKEJOBS) -Q -C test || return 1; \
	find $(BUILDPATH)/$(MODULE)/test/coverage -name "*.gcda" -delete; \
	$(BUILDPATH)/$(MODULE)/test/coverage/runner; \
	mkdir -p $(BUILDPATH)/coverage; \
	cd $(BUILDPATH)/$(MODULE)/test/coverage; \
	gcovr --root=. --object-directory=. $(foreach dir,$(LCOV_REMOVE_PATTERN), -e $(dir)) --xml-pretty -o $(BUILDPATH_ABSOLUTE)/coverage/$(MODULE).xml

coverage-html-default: coverage-default
	@cd $(BUILDPATH)/$(MODULE)/test/coverage; \
	lcov -t '$(MODULE) library' -o coverage.info -c --base-directory $(CURDIR)/test --directory . --gcov-tool `which gcov` || return 1; \
	lcov -r coverage.info $(LCOV_DEFAULT_REMOVE_PATTERN) $(LCOV_REMOVE_PATTERN) -o coverage.info; \
	rm -rf report; \
	genhtml --demangle-cpp -o report coverage.info

coverage-view-default:
	@xdg-open $(BUILDPATH)/$(MODULE)/test/coverage/report/index.html &

cloc-source-default:
	@mkdir -p $(BUILDPATH)/cloc
	@cloc --by-file --xml --out=$(BUILDPATH)/cloc/$(MODULE).xml src/

# Run clang static analyzer (see http://clang-analyzer.llvm.org/). Requires
# that the unittests are configured in the SConstruct file to be build
# with the 'hosted-clang' module.
analyze-clang:
	@PATH=$(PATH):~/Downloads/llvm/tools/clang/tools/scan-build \
	scan-build --use-analyzer "$(shell which clang)" \
	-o "../../build/$(MODULE)/test/analyze-clang" \
	scons build $(MAKEJOBS) analyze=1 -Q -C test/unit

analyze-clang-view:
	@PATH=$(PATH):~/Downloads/llvm/tools/clang/tools/scan-view \
	scan-view "$(CURDIR)/../../build/$(MODULE)/test/analyze-clang/$(FOLDER)"

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

design:
	@$(MAKE) -C doc/design

doc: doxygen design

CLANG_FORMAT="clang-format-6.0"

FORMAT_SOURCE_FILES ?= $(shell find src/ test/ -type f -name '*.cpp')
FORMAT_HEADER_FILES ?= $(shell find src/ test/ -type f -name '*.h')

format:
	@$(CLANG_FORMAT) -i $(FORMAT_SOURCE_FILES)
	@$(CLANG_FORMAT) -i $(FORMAT_HEADER_FILES)

clean-default:
	@$(RM) -r doc/doxygen/api/*
	@$(RM) -r $(BUILDPATH)/$(MODULE)/test/unittest
	@$(RM) -r $(BUILDPATH)/$(MODULE)/test/coverage
	@$(RM) -r $(BUILDPATH)/test/$(MODULE).xml
	@$(RM) -r $(BUILDPATH)/cloc/$(MODULE).xml
	@$(RM) -r $(BUILDPATH)/coverage/$(MODULE).xml

distclean-default:
	@scons build coverage=1 -Q -C test/unit -c

.PHONY: build-lua test-default coverage-default coverage-html-default cppcheck cppcheck-tests cppcheck-unittests
