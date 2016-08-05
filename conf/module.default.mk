# Copyright (c) 2013, German Aerospace Center (DLR)
# All Rights Reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 
# * Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
# * Neither the name of the German Aerospace Center (DLR) nor the
#   names of its contributors may be used to endorse or promote products
#   derived from this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

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
	@find src/ -regex ".*\.\(h\|cpp\)" | vera++ -p cobc --show-rule --summary --root ../../tools/vera++

codingstyle-jsf:
	@$(POLYSPACE)/QuickPS.sh -d "src/" \
	                         -c "$(POLYSPACE)/profiles/$(MODULE)/options.cfg" \
	                         -o "$(POLYSPACE)/profiles/$(MODULE)/options.txt" \
	                         -i "$(POLYSPACE)/profiles/polyspace_header.h" \
	                         -p "$(POLYSPACE)/ssh_info.txt" \
	                         -x

codingstyle-jsf-python:
	@$(POLYSPACE)/python/QuickPS.py analyse "src/" \
	                         --config "$(POLYSPACE)/profiles/$(MODULE)/options_py.cfg" \
	                         --options "$(POLYSPACE)/profiles/$(MODULE)/options.txt" \
	                         --include "$(POLYSPACE)/profiles/polyspace_header.h" \
	                         --ssh "$(POLYSPACE)/python/ssh_info_dlr.ini" \
	                         -a -x

codingstyle-jsf-view:
	@$(POLYSPACE)/polyspace_jsf_log_formater.py $(POLYSPACE)/results/$(MODULE)/PolySpace_C_R2009a_src_latest.log \
	                                            $(POLYSPACE)/results/$(MODULE)/PolySpace_C_R2009a_src_latest.1.log
	@xdg-open $(POLYSPACE)/results/$(MODULE)/PolySpace_C_R2009a_src_latest.1.log &

codingstyle-jsf-annotate:
	@$(POLYSPACE)/polyspace_jsf_annotate.py -d "src/" \
	                                        -c "$(POLYSPACE)/profiles/$(MODULE)/options.cfg" \
	                                        -r "$(POLYSPACE)/results/$(MODULE)/JSF-report.xml" \
	                                        $(POLYSPACE_RULES) \
	                                        --annotate 

codingstyle-jsf-remove-annotations:
	@$(POLYSPACE)/polyspace_jsf_annotate.py -d "src/" \
	                                        -c "$(POLYSPACE)/profiles/$(MODULE)/options.cfg" \
	                                        -r "$(POLYSPACE)/results/$(MODULE)/JSF-report.xml"

doxygen:
	@doxygen doc/doxygen/doxyfile

design:
	@$(MAKE) -C doc/design

doc: doxygen design

clean-default:
	@$(RM) -r doc/doxygen/api/*
	@$(RM) -r $(BUILDPATH)/$(MODULE)/test/unittest
	@$(RM) -r $(BUILDPATH)/$(MODULE)/test/coverage
	@$(RM) -r $(BUILDPATH)/test/$(MODULE).xml
	@$(RM) -r $(BUILDPATH)/cloc/$(MODULE).xml
	@$(RM) -r $(BUILDPATH)/coverage/$(MODULE).xml

distclean-default:
	@scons build coverage=1 -Q -C test/unit -c

.PHONY: build-lua test-default coverage-default coverage-html-default
