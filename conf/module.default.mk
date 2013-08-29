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
# Default is 3 parallel jobs. Use `MAKEJOBS=` to disable parallel build.
MAKEJOBS=-j3
endif

# Run style checker
style:
	@find src/ -regex ".*\.\(h\|cpp\)" | vera++ -p cobc --show-rule --summary --root ../../tools/vera++

doxygen:
	@doxygen doc/doxygen/doxyfile

design:
	@$(MAKE) -C doc/design

doc: doxygen design

clean_default:
	@$(MAKE) -C doc/design clean
	@$(RM) -r doc/doxygen/api/*

.PHONY: doxygen design doc clean_default
