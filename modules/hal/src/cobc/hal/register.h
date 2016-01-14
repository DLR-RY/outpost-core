/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of the project 'ccsds_rt', version 0.1.
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

#ifndef COBC_HAL_REGISTER_H
#define COBC_HAL_REGISTER_H

#include <stdint.h>
#include <cobc/utils/bit_access.h>

namespace cobc
{
namespace hal
{

/**
 * Register access.
 *
 * Provides methods to read and write complete registers as well as bit fields
 * inside a 32 bit register.
 *
 * \author  Fabian Greif
 */
class Register
{
public:
    template <typename RegisterType, int offsetStart, int offsetEnd, uint32_t registerAddress>
    struct Bitfield
    {
        typedef RegisterType Type;
        static const int start = offsetStart;
        static const int end = offsetEnd;
        static const uint32_t address = registerAddress;
    };

    template <typename RegisterType, int offset, uint32_t registerAddress>
    struct SingleBit
    {
        typedef RegisterType Type;
        static const int start = offset;
        static const int end = offset;
        static const uint32_t address = registerAddress;
    };

    /**
     * Access a register.
     *
     * Allows for 8, 16 or 32 bit access. For example:
     * \code
     * uint16_t value = Register::access<uint16_t>(0x00000000);
     * \endcode
     *
     * \tparam T
     *      Register type. Valid types are uint8_t, uint16_t and uint32_t.
     * \param address
     *      Address of the register.
     * \return
     *      Access object for the register.
     */
    template <typename T>
    static inline volatile T&
    access(uint32_t address);

    template <typename T>
    static inline T*
    getPointer(uint32_t address);

    template <typename T>
    static inline uint32_t
    read();

    template <typename T>
    static inline uint32_t
    readWithOffset(size_t offset);

    template <typename T>
    static inline uint32_t
    readFromMemory(uint32_t& memory);

    template <typename T>
    static inline void
    write(uint32_t value);

    /**
     * Write the value directly into the register without reading it first.
     *
     * All other bits apart from the one provided through \c value are set
     * to zero.
     *
     * \warning This might overwrite other values already stored in
     *          the register!
     */
    template <typename T>
    static inline void
    overwrite(uint32_t value);

    template <typename T>
    static inline void
    writeWithOffset(uint32_t value, size_t offset);

    template <typename T>
    static inline void
    writeToMemory(uint32_t value, uint32_t& memory);

    template <typename T>
    static inline uint32_t
    getValue(uint32_t value);

    template <typename T>
    static inline uint32_t
    getMask();

private:
    // Disable destructor, copy-constructor and copy assignment operator
    Register();

    ~Register();

    Register(const Register&);

    Register&
    operator=(const Register&);
};

}
}

#include "register_impl.h"

#endif
