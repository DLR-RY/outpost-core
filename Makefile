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

MODULES = ccsds pus rtos smpc time utils

# Color definitions
# TODO disable these if the console is not able to output color
CINFO  = \033[;0;33m
COK    = \033[32;01m
CERROR = \033[31;01m
CEND   = \033[0m

help:
	@echo "Please use \`make <target>\` where <target> is one of"
	@echo "  doc      to build all documentation"
	@echo "  design   to build the design documentation (requires xelatex)"
	@echo "  doxygen  to build the doxygen documentation"
	@echo "  test     to run all unit tests"
	@echo "  style    to check the coding style with vera++"
	@echo "  clean    to remove temporary data (\`build\` folder)"

doc: design doxygen

documents: design

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

