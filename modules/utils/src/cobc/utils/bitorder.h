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

namespace cobc
{

template <typename T, int startIn, int endIn>
class BitorderConverter
{
protected:
    static const int offsetByte = startIn & 7;

    static const int numberOfBitsPerByte = 8;
    static const int msb = sizeof(T) * numberOfBitsPerByte - 1;

public:
    /// Byte index of the first value in the byte array
    static const size_t index = startIn / 8;
    static const int width = endIn - startIn + 1;

    static const int start = msb - offsetByte;
    static const int end = msb - (endIn - index * numberOfBitsPerByte);
};

}

#endif // COBC_UTILS_BITORDER_H
