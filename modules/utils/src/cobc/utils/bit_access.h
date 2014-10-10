/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.3-pre.
 *
 * It is distributed under the terms of the GNU General Public License with
 * a linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
// ----------------------------------------------------------------------------

#ifndef COBC_UTILS_BIT_ACCESS_H
#define COBC_UTILS_BIT_ACCESS_H

#include <stddef.h>
#include <stdint.h>

namespace cobc
{

/**
 * Bit field access.
 *
 * Provides access to single or multiple bits in a larger variable.
 *
 * Uses LSB 0 bit numbering (bit 0 is the LSB).
 *
 * \author  Fabian Greif
 */
class BitAccess
{
public:
    /**
     * Get a single bit.
     *
     * \tparam T
     *      Register type. Valid types are uint8_t, uint16_t and uint32_t.
     * \tparam offset
     *      Offset of the bit within the register.
     *
     * \param data
     *      Value from which to extract the bit value.
     *
     * \retval true   Bit is set.
     * \retval false  Bit is cleared.
     */
    template <typename T, int offset>
    static bool
    get(const T& data);

    /**
     * Read multiple bits within a value.
     *
     * \tparam T
     *      Register type. Valid types are uint8_t, uint16_t and uint32_t.
     * \tparam start
     *      Start of the bitfield to read.
     * \tparam end
     *      End of the bitfield to read.
     *
     * \param data
     *      Value from which to extract the bitfield value.
     *
     * \return
     *      Value of the bits in the field defined through \c offset and \c width.
     */
    template <typename T, int start, int end>
    static T
    get(const T& data);

    /**
     * Set a single bit.
     *
     * \tparam T
     *      Register type. Valid types are uint8_t, uint16_t and uint32_t.
     * \tparam offset
     *      Offset of the bit within the register.
     *
     * \param address
     *      Address of the register.
     * \param value
     *      Set bit if \p value is true, otherwise clear it.
     */
    template <typename T, int offset>
    static void
    set(T& data, bool value);

    /**
     * Set multiple bits within a register.
     *
     * \tparam T
     *      Register type. Valid types are uint8_t, uint16_t and uint32_t.
     * \tparam start
     *      Start of the bitfield to write.
     * \tparam end
     *      End of the bitfield to write.
     *
     * \param address
     *      Address of the register.
     * \param value
     *      Value to set in the field defined through \c offset and \c width.
     */
    template <typename T, int start, int end>
    static void
    set(T& data, T value);

    template <typename T>
    static T
    getMask(size_t width);
};

}

#include "bit_access_impl.h"

#endif // COBC_UTILS_BIT_ACCESS_H
