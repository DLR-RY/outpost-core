/*
 * Copyright (c) 2016-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2016-2017, Fabian Greif (DLR RY-AVS)
 */

#ifndef OUTPOST_HAL_REGISTER_IMPL_H
#define OUTPOST_HAL_REGISTER_IMPL_H

#include "register.h"

// ----------------------------------------------------------------------------
template <typename T>
volatile T&
outpost::hal::Register::access(uint32_t address)
{
    volatile T* reg = reinterpret_cast<volatile T*>(address);
    return *reg;
}

template <typename T>
T*
outpost::hal::Register::getPointer(uint32_t address)
{
    T* ptr = reinterpret_cast<T*>(address);
    return ptr;
}

template <typename T>
uint32_t
outpost::hal::Register::read()
{
    typename T::Type reg = access<typename T::Type>(T::address);
    typename T::Type value = outpost::BitAccess::get<typename T::Type, T::start, T::end>(reg);
    return value;
}

template <typename T>
uint32_t
outpost::hal::Register::readWithOffset(size_t offset)
{
    const uint32_t address = T::address + (offset * sizeof(typename T::Type));
    typename T::Type registerValue = access<typename T::Type>(address);
    typename T::Type value =
            outpost::BitAccess::get<typename T::Type, T::start, T::end>(registerValue);
    return value;
}

template <typename T>
uint32_t
outpost::hal::Register::readFromMemory(uint32_t& memory)
{
    typename T::Type value = outpost::BitAccess::get<typename T::Type, T::start, T::end>(memory);
    return value;
}

template <typename T>
void
outpost::hal::Register::write(uint32_t value)
{
    typename T::Type registerValue = access<typename T::Type>(T::address);
    outpost::BitAccess::set<typename T::Type, T::start, T::end>(registerValue, value);
    access<typename T::Type>(T::address) = registerValue;
}

template <typename T>
void
outpost::hal::Register::overwrite(uint32_t value)
{
    typename T::Type registerValue = 0;
    // TODO this does an unnecessary clear of the register first.
    outpost::BitAccess::set<typename T::Type, T::start, T::end>(registerValue, value);
    access<typename T::Type>(T::address) = registerValue;
}

template <typename T>
void
outpost::hal::Register::writeWithOffset(uint32_t value, size_t offset)
{
    const uint32_t address = T::address + (offset * sizeof(typename T::Type));
    typename T::Type registerValue = access<typename T::Type>(address);
    outpost::BitAccess::set<typename T::Type, T::start, T::end>(registerValue, value);
    Register::access<typename T::Type>(address) = registerValue;
}

template <typename T>
void
outpost::hal::Register::writeToMemory(uint32_t value, uint32_t& memory)
{
    outpost::BitAccess::set<typename T::Type, T::start, T::end>(memory, value);
}

template <typename T>
uint32_t
outpost::hal::Register::getValue(uint32_t value)
{
    const int width = (T::end - T::start) + 1;
    typename T::Type mask = outpost::BitAccess::getMask<typename T::Type>(width);
    uint32_t t = (value & mask) << T::start;
    return t;
}

template <typename T>
uint32_t
outpost::hal::Register::getMask()
{
    const int width = (T::end - T::start) + 1;
    typename T::Type mask = outpost::BitAccess::getMask<typename T::Type>(width);
    mask <<= T::start;
    return mask;
}

#endif
