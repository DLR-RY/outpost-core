
BOARD=5

build:
	@scons install

program: build
	@./program.sh bin/rtems.elf /dev/outpost_dsu_$(BOARD)

terminal-console:
	@picocom -b38400 /dev/outpost_console_$(BOARD)

terminal-link:
	@picocom -b38400 /dev/outpost_link_$(BOARD)

