/*
 * Copyright (c) 2014-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#ifndef OUTPOST_UTILS_BITORDER_H
#define OUTPOST_UTILS_BITORDER_H

#include "helper.h"

namespace outpost
{

/**
 * Conversion from MSB0 to LSB0 bit-ordering
 *
 * \tparam  T
 *      Target value type. Can be uint8|16|32_t.
 * \tparam startIn
 *      First bit of bitfield inside a byte array.
 * \tparam endIn
 *      Last bit of bitfield inside a byte array.
 *
 * \author  Fabian Greif
 */
template <typename T, int startIn, int endIn>
class BitorderMsb0ToLsb0
{
protected:
    // static assert only possible in C++11 :(
    //static_assert(startIn < endIn, "Invalid bitfield definition! 'startIn' must be smaller than 'endIn'");

    static const int numberOfBitsPerByte = 8;
    static const int msb = sizeof(T) * numberOfBitsPerByte - 1;

    static const int offsetByte = startIn & (numberOfBitsPerByte - 1);

public:
    /**
     * Byte index of the first byte of the target value in the byte array.
     */
    static const size_t byteIndex = startIn / numberOfBitsPerByte;
    static const int width = (endIn - startIn) + 1;

    /**
     * First bit of the target value inside the field given by T.
     *
     * As this is in LSB0 ordering it is always 'start > end'.
     */
    static const int start = msb - offsetByte;

    /**
     * Last bit of the target value inside the field given by T.
     */
    static const int end = msb - (endIn - (byteIndex * numberOfBitsPerByte));
};

}

#endif
