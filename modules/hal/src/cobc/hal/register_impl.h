/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.6.
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

#ifndef COBC_HAL_REGISTER_IMPL_H
#define COBC_HAL_REGISTER_IMPL_H

#include "register.h"

// ----------------------------------------------------------------------------
template <typename T>
volatile T&
cobc::hal::Register::access(uint32_t address)
{
    volatile T* reg = reinterpret_cast<volatile T*>(address);
    return *reg;
}

template <typename T>
T*
cobc::hal::Register::getPointer(uint32_t address)
{
    T* ptr = reinterpret_cast<T*>(address);
    return ptr;
}

template <typename T>
uint32_t
cobc::hal::Register::read()
{
    typename T::Type reg = access<typename T::Type>(T::address);
    typename T::Type value = cobc::BitAccess::get<typename T::Type, T::start, T::end>(reg);
    return value;
}

template <typename T>
uint32_t
cobc::hal::Register::readWithOffset(size_t offset)
{
    const uint32_t address = T::address + (offset * sizeof(typename T::Type));
    typename T::Type registerValue = access<typename T::Type>(address);
    typename T::Type value = cobc::BitAccess::get<typename T::Type, T::start, T::end>(registerValue);
    return value;
}

template <typename T>
uint32_t
cobc::hal::Register::readFromMemory(uint32_t& memory)
{
    typename T::Type value = cobc::BitAccess::get<typename T::Type, T::start, T::end>(memory);
    return value;
}

template <typename T>
void
cobc::hal::Register::write(uint32_t value)
{
    typename T::Type registerValue = access<typename T::Type>(T::address);
    cobc::BitAccess::set<typename T::Type, T::start, T::end>(registerValue, value);
    access<typename T::Type>(T::address) = registerValue;
}

template <typename T>
void
cobc::hal::Register::overwrite(uint32_t value)
{
    typename T::Type registerValue = 0;
    // TODO this does an unnecessary clear of the register first.
    cobc::BitAccess::set<typename T::Type, T::start, T::end>(registerValue, value);
    access<typename T::Type>(T::address) = registerValue;
}

template <typename T>
void
cobc::hal::Register::writeWithOffset(uint32_t value, size_t offset)
{
    const uint32_t address = T::address + (offset * sizeof(typename T::Type));
    typename T::Type registerValue = access<typename T::Type>(address);
    cobc::BitAccess::set<typename T::Type, T::start, T::end>(registerValue, value);
    Register::access<typename T::Type>(address) = registerValue;
}

template <typename T>
void
cobc::hal::Register::writeToMemory(uint32_t value, uint32_t& memory)
{
    cobc::BitAccess::set<typename T::Type, T::start, T::end>(memory, value);
}

template <typename T>
uint32_t
cobc::hal::Register::getValue(uint32_t value)
{
    const int width = (T::end - T::start) + 1;
    typename T::Type mask = cobc::BitAccess::getMask<typename T::Type>(width);
    uint32_t t = (value & mask) << T::start;
    return t;
}

template <typename T>
uint32_t
cobc::hal::Register::getMask()
{
    const int width = (T::end - T::start) + 1;
    typename T::Type mask = cobc::BitAccess::getMask<typename T::Type>(width);
    mask <<= T::start;
    return mask;
}

#endif
