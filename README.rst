
.. image:: doc/images/OUTPOST_logo.svg
   :width: 100px
   :alt: OUTPOST logo
   :align: center

=======
OUTPOST
=======

The OUTPOST library (Open modUlar sofTware PlatfOrm for SpacecrafT)
is developed at the `German Aerospace Center (DLR)`_ and provides an
execution platform targeted at embedded systems running mission
critical software.
The library set up to be modular, allowing the user to choose which
modules to use and which modules to leave out.

OUTPOST is split into two repositories `outpost-core` contaning the base
environment and `outpost-satellite` containing the satellite specific
modules.

This repository (`outpost-core`) contains the following software modules:

utils
    A collection of useful bits and pieces. The reuseable unit
    test harness is located here.

time
    Type safe time management functions. Provides classes to convert
    between time representations, e.g. Unix Time to GPS or TAI.

rtos
    RTOS wrapper layer. Allows to build OUTPOST on different operating
    system. Supports RTEMS, FreeRTOS and all POSIX compatible OS.

hal
    Hardware abstraction layer for communication interfaces.

smpc
    Simple Message Passing Channel. Provides a very basic, but fast
    publish/subscribe based communication middleware for objects living in
    the same address space.

l3test
    Lua based testing framework. Allows to run Lua scripts as part of
    the unit tests.


.. _German Aerospace Center (DLR): http://www.dlr.de/irs/en/
