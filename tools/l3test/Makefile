
test:
	@scons -Q -C test/unit -j3
	@./build/test/unit/unittest

coverage:
	@scons -Q -C test/unit coverage=1 -j3
	@./build/test/coverage/unittest

.PHONY: test

