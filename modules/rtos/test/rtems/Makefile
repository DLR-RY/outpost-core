
BOARD=5

build:
	@scons install

program: build
	@./program.sh bin/rtems.elf /dev/cobc_dsu_$(BOARD)

terminal-console:
	@picocom -b38400 /dev/cobc_console_$(BOARD)

terminal-link:
	@picocom -b38400 /dev/cobc_link_$(BOARD)

