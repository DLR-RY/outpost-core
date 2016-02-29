=======
libCOBC
=======

Library for Communication & Services
====================================

This repository contains the following software modules:

spp
	CCSDS module for encoding/decoding transfer frames. Contains a backend
	for tunneling CCSDS via UART for testing purposes.

pus
	PUS (Packet Utilization Standard) module. Contains some of the standard
	services.

rtos
	RTOS Wrapper and hardware abstraction layer. Currently only RTEMS and all
	POSIX compatible OS are supported but other OS may be added as well.

smpc
	Simple Message Passing Channel. Provides a very basic, but fast
	publish/subscribe based communication middleware for objects living in
	the same address space.

utils
	Small utilities. Not a real software module rather a collection of
	useful bits and pieces. Some of the reuseable unit test harness are
	located here.


Folder structure
----------------

TBW. <----------------------------

README and INSTALL as well as all other documentation should use
`reStructuredText`_ for formatting. This allows to generate nice looking
documents form them by converting them to HTML or LaTeX.


Naming conventions
------------------

Unit tests
~~~~~~~~~~

Fake...
	Implements an interface but does little more than for example add a message
	to a trace log, against which a test assertion can be run.

Mock...
	Mock objects may contain test assertion that can make the test fail, for
	example if some data is not as expected.

Testing...
	Simple subclass of the class under test, e.g. to expose interna or to add
	.

...Test
	Test suite to test a given class.


Requirements
------------

To build the unittests the following programs in their respective versions
are needed:

- gcc >= 4.7
- scons >= 2.3
- python >= 2.7
- clang >= 3.2
- lcov >= 1.10

For Debian-based Linux systems (e.g. Debian 8)::

    sudo apt-get install clang llvm scons libboost-dev libsqlite3-dev curl lcov
    
    curl -R -O http://www.lua.org/ftp/lua-5.3.0.tar.gz
    tar zxf lua-5.3.0.tar.gz
    cd lua-5.3.0
    make linux test
    sudo make install
    
    wget http://luarocks.org/releases/luarocks-2.2.1.tar.gz
    tar zxpf luarocks-2.2.1.tar.gz
    cd luarocks-2.2.1
    ./configure
    sudo make bootstrap
    
    sudo luarocks install busted


.. _reStructuredText: http://docutils.sourceforge.net/docs/user/rst/quickref.html
