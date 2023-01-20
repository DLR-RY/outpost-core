<img src="doc/images/OUTPOST_logo.png" height="200px" />

The **OUTPOST** library (Open modUlar sofTware PlatfOrm for SpacecrafT)
is developed at the [German Aerospace Center (DLR)][] and provides an
execution platform targeted at embedded systems running mission
critical software.
The library set up to be modular, allowing the user to choose which
modules to use and which modules to leave out.

OUTPOST is split into two repositories: *outpost-core* contains the open-source
base environment and *outpost-satellite* contains the closed-source satellite
specific modules.

IMPORTANT NOTICE
-------

This repositry is only a snapshot of the current state of development in progress at DLR.
If you want to participate or are interested in the satellite specific modules, please contact
jan-gerd.mess[at]dlr.de


Modules
-------

This repository contains the following software modules:
- `base`

    Most basic data structures and arithmetic helper classes used throughout the library.

- `utils`

    A collection of useful bits and pieces. The reuseable unit
    test harness is located here.
    
- `comm`

    Implementation of the Remote Memory Access Protocol (RMAP) via SpaceWire.
    
- `compression`

    Algorithms for (de)compression and de-/encoding of time-series data using wavelet compression and 
    set-partitioning-in-hierarchical-trees.

- `time`

    Type safe time management functions. Provides classes to convert
    between time representations, e.g. Unix Time to GPS or TAI.

- `parameter`

   Provides system-wide access to variables that need to be read/written by different
   parts of the (loosely coupled) system.
    
- `rtos`

    RTOS wrapper layer. Allows to build OUTPOST on different operating
    system. Supports RTEMS, FreeRTOS and all POSIX compatible OS.

- `hal`

    Hardware abstraction layer for communication interfaces.

- `sip`

    Simple Interface Protocol. A minimal packet-based communication protocol for hardware links,
    includes specifications for UART, SpaceWire and UDP
    
- `smpc`

    Simple Message Passing Channel. Provides a very basic, but fast
    publish/subscribe based communication middleware for objects living in
    the same address space.
    
- `swb`

    Software Bus. An asynchronous mean of communication between software modules based on the publish/subscribe paradigm.

- `support`

    A heartbeat-mechanism that serves as a software watchdog on thread-level.


Installation
------------

The installation of the tool required for the library development
is described [in the INSTALL file](INSTALL.md).


License
-------

Unless otherwise noted the source code is licensed under the
terms of the *Mozilla Public License, v. 2.0* (see mpl-v2.0.txt).

Files with a different license:

 - tools/l3test/src/lua/exception.h (BSD 3-clause)
 - modules/utils/ext/rapidcheck/* (BSD 2-clause)
 - modules/utils/ext/googletest-1.8.0-fused/* (BSD 3-clause)


[German Aerospace Center (DLR)]: http://www.dlr.de/irs/en/
