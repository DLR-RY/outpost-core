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

# Filter for the Unittest runners
FILTER := *

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

# Path relative from the module folders (e.g. '/trunk/modules/pus')
POLYSPACE ?= ../../tools/polyspace

ifneq ($(RULES),)
  POLYSPACE_RULES=--rules="$(RULES)"
else
  POLYSPACE_RULES=
endif

# Run clang static analyzer (see http://clang-analyzer.llvm.org/). Requires
# that the unittests are configured in the SConstruct file to be build
# with the 'hosted-clang' module.
analyze-clang:
	@PATH=$(PATH):~/Downloads/llvm/tools/clang/tools/scan-build \
	scan-build --use-analyzer "$(shell which clang)" \
	-o "../../build/$(MODULE)/test/analyze-clang" \
	scons build $(MAKEJOBS) analyze=1 -Q -C test/unit

analyze-clang-view:
	PATH=$(PATH):~/Downloads/llvm/tools/clang/tools/scan-view \
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

clean_default:
	@$(RM) -r doc/doxygen/api/*

distclean_default:
	@scons build coverage=1 -Q -C test/unit -c

.PHONY: doxygen design doc clean_default distclean_default

