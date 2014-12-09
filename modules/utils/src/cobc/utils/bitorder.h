/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.3-pre.
 *
 * It is distributed under the terms of the GNU General Public License with a
 * linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */
// ----------------------------------------------------------------------------

#ifndef COBC_UTILS_BITORDER_H
#define COBC_UTILS_BITORDER_H

#include "helper.h"

namespace cobc
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
    static_assert(startIn < endIn, "Invalid bitfield definition! 'startIn' must be smaller than 'endIn'");

    static const int offsetByte = startIn & 7;

    static const int numberOfBitsPerByte = 8;
    static const int msb = sizeof(T) * numberOfBitsPerByte - 1;

public:
    /**
     * Byte index of the first byte of the target value in the byte array.
     */
    static const size_t byteIndex = startIn / 8;
    static const int width = endIn - startIn + 1;

    /**
     * First bit of the target value inside the field given by T.
     *
     * As this is in LSB0 ordering it is always 'start > end'.
     */
    static const int start = msb - offsetByte;

    /**
     * Last bit of the target value inside the field given by T.
     */
    static const int end = msb - (endIn - byteIndex * numberOfBitsPerByte);
};

}

#endif // COBC_UTILS_BITORDER_H
