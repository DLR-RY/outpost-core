/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef COBC_UTILS_BIT_ACCESS_H
#define COBC_UTILS_BIT_ACCESS_H

#include <stdint.h>

namespace cobc
{

/**
 * Bit field access.
 *
 * Provides access to single or multiple bits in a larger variable.
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

private:
    template <typename T>
    static T
    createMask(int width);
};

}

#include "bit_access_impl.h"

#endif // COBC_UTILS_BIT_ACCESS_H
