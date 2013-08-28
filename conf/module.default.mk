
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
	@$(RM) doc/doxygen/api/*

.PHONY: doxygen design doc clean_default
