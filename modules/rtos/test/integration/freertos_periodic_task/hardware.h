
#ifndef HARDWARE_H
#define HARDWARE_H

#include <xpcc/architecture.hpp>

GPIO__OUTPUT(LedOrange, D, 13);        // User LED 3
GPIO__OUTPUT(LedGreen, D, 12);        // User LED 4
GPIO__OUTPUT(LedRed, D, 14);        // User LED 5
GPIO__OUTPUT(LedBlue, D, 15);        // User LED 6

#endif // HARDWARE_H
