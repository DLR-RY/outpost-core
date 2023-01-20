/*
 * Copyright (c) 2020-2021, German Aerospace Center (DLR)
 * All rights reserved.
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2020-2021, Jan Malburg (DLR RY-AVS)
 */

/**
 * A set of class templates to store (unsigned) integer values of bit-width not provided by
 * regular C++
 */
#ifndef OUTPOST_UTILS_STORAGE_INT_H_
#define OUTPOST_UTILS_STORAGE_INT_H_

#include "big_endian_int.h"

#include <limits>
#include <stddef.h>
#include <stdint.h>

#include <type_traits>

namespace outpost
{
namespace utils
{
/*
 * for signed ones
 */
template <size_t bits, typename enable = void>
class Integer
{
};

/**
 * Helper class to check for a type
 */
class IntegerBase
{
    template <size_t bits, typename enable>
    friend class Integer;

private:
    /**
     * Ensure only friends can be children
     */
    IntegerBase() = default;
};

template <size_t bits>
class Integer<bits, typename std::enable_if<(bits > 0) && (bits <= 8)>::type> : private IntegerBase
{
public:
    using PrimitiveType = int8_t;

    /**
     * For the case you just need an object of the type
     */
    Integer() = default;

    // cppcheck-suppress noExplicitConstructor
    Integer(int8_t data) : mData(data)
    {
        limitToRange();
    }

    Integer&
    operator=(int8_t data)
    {
        mData = data;
        limitToRange();
        return *this;
    }

    explicit operator int8_t() const
    {
        return mData;
    }

    explicit operator int16_t() const
    {
        return mData;
    }

    explicit operator int32_t() const
    {
        return mData;
    }

    explicit operator int64_t() const
    {
        return mData;
    }

    /**
     * @return the bit width of this type
     */
    static constexpr size_t
    getBits()
    {
        return bits;
    }

    int8_t
    toPrimitiveType() const
    {
        return static_cast<int8_t>(*this);
    }

    bool
    operator==(const Integer& o) const
    {
        return mData == o.mData;
    }

    bool
    operator!=(const Integer& o) const
    {
        return !(*this == o);
    }

    // pre-* versions are enough, intended for the case we want to have them as loop variable

    Integer&
    operator++()
    {
        ++mData;
        limitToRange();
        return *this;
    }

    Integer&
    operator--()
    {
        --mData;
        limitToRange();
        return *this;
    }

    Integer&
    operator+=(const Integer& o)
    {
        mData += o.mData;
        limitToRange();
        return *this;
    }

    Integer&
    operator+=(const PrimitiveType& o)
    {
        mData += o;
        limitToRange();
        return *this;
    }

    Integer&
    operator-=(const Integer& o)
    {
        mData -= o.mData;
        limitToRange();
        return *this;
    }

    Integer&
    operator-=(const PrimitiveType& o)
    {
        mData -= o;
        limitToRange();
        return *this;
    }

    Integer
    operator+(const Integer& o)
    {
        return Integer(mData + o.mData);
    }

    Integer
    operator-(const Integer& o)
    {
        return Integer(mData - o.mData);
    }

    Integer
    operator&(const Integer& o) const
    {
        Integer ret;
        ret.mData = o.mData & mData;
        return ret;
    }

    Integer&
    operator&=(const Integer& o)
    {
        mData = o.mData & mData;
        return *this;
    }

    Integer
    operator|(const Integer& o) const
    {
        Integer ret;
        ret.mData = o.mData | mData;
        return ret;
    }

    Integer&
    operator|=(const Integer& o)
    {
        mData = o.mData | mData;
        return *this;
    }

    static constexpr PrimitiveType maxValue =
            static_cast<PrimitiveType>(static_cast<uint8_t>(UINT8_MAX >> (8 - (bits - 1))));
    static constexpr PrimitiveType minValue =
            static_cast<PrimitiveType>(static_cast<uint8_t>(UINT8_MAX & ~maxValue));

private:
    static constexpr PrimitiveType mask =
            static_cast<PrimitiveType>(static_cast<uint8_t>(UINT8_MAX >> (8 - bits)));

    void
    limitToRange()
    {
        if (mData & (1 << (bits - 1)))
        {
            mData |= ~mask;
        }
        else
        {
            mData &= mask;
        }
    }

    int8_t mData = 0;
};

template <size_t bits>
class Integer<bits, typename std::enable_if<(bits > 8) && (bits <= 16)>::type> : private IntegerBase
{
public:
    using PrimitiveType = int16_t;

    /**
     * For the case you just need an object of the type
     */
    Integer() = default;

    // cppcheck-suppress noExplicitConstructor
    Integer(int16_t data) : mData(data)
    {
        limitToRange();
    }

    Integer&
    operator=(int16_t data)
    {
        mData = data;
        limitToRange();
        return *this;
    }

    explicit operator int16_t() const
    {
        return mData;
    }

    explicit operator int32_t() const
    {
        return mData;
    }

    explicit operator int64_t() const
    {
        return mData;
    }

    /**
     * @return the bit width of this type
     */
    static constexpr size_t
    getBits()
    {
        return bits;
    }

    int16_t
    toPrimitiveType() const
    {
        return static_cast<int16_t>(*this);
    }

    bool
    operator==(const Integer& o) const
    {
        return mData == o.mData;
    }

    bool
    operator!=(const Integer& o) const
    {
        return !(*this == o);
    }

    // pre-* versions are enough, intended for the case we want to have them as loop variable

    Integer&
    operator++()
    {
        ++mData;
        limitToRange();
        return *this;
    }

    Integer&
    operator--()
    {
        --mData;
        limitToRange();
        return *this;
    }

    Integer&
    operator+=(const Integer& o)
    {
        mData += o.mData;
        limitToRange();
        return *this;
    }

    Integer&
    operator+=(const PrimitiveType& o)
    {
        mData += o;
        limitToRange();
        return *this;
    }

    Integer&
    operator-=(const Integer& o)
    {
        mData -= o.mData;
        limitToRange();
        return *this;
    }

    Integer&
    operator-=(const PrimitiveType& o)
    {
        mData -= o;
        limitToRange();
        return *this;
    }

    Integer
    operator+(const Integer& o)
    {
        return Integer(mData + o.mData);
    }

    Integer
    operator-(const Integer& o)
    {
        return Integer(mData - o.mData);
    }

    Integer
    operator&(const Integer& o) const
    {
        Integer ret;
        ret.mData = o.mData & mData;
        return ret;
    }

    Integer&
    operator&=(const Integer& o)
    {
        mData = o.mData & mData;
        return *this;
    }

    Integer
    operator|(const Integer& o) const
    {
        Integer ret;
        ret.mData = o.mData | mData;
        return ret;
    }

    Integer&
    operator|=(const Integer& o)
    {
        mData = o.mData | mData;
        return *this;
    }

    static constexpr PrimitiveType maxValue =
            static_cast<PrimitiveType>(static_cast<uint16_t>(UINT16_MAX >> (16 - (bits - 1))));
    static constexpr PrimitiveType minValue =
            static_cast<PrimitiveType>(static_cast<uint16_t>(UINT16_MAX & ~maxValue));

private:
    static constexpr PrimitiveType mask =
            static_cast<PrimitiveType>(static_cast<uint16_t>(UINT16_MAX >> (16 - bits)));

    void
    limitToRange()
    {
        if (mData & (1 << (bits - 1)))
        {
            mData |= ~mask;
        }
        else
        {
            mData &= mask;
        }
    }

    int16_t mData = 0;
};

template <size_t bits>
class Integer<bits, typename std::enable_if<(bits > 16) && (bits <= 32)>::type>
    : private IntegerBase
{
public:
    using PrimitiveType = int32_t;

    /**
     * For the case you just need an object of the type
     */
    Integer() = default;

    // cppcheck-suppress noExplicitConstructor
    Integer(int32_t data)
    {
        if (data & (1 << (bits - 1)))
        {
            data |= ~mask;
        }
        else
        {
            data &= mask;
        }

        uint8_t* sourcePointer = reinterpret_cast<uint8_t*>(&data);
        if (isBigEndian())
        {
            for (size_t i = 4 - size; i < 4; i++)
            {
                mData[i - (4 - size)] = sourcePointer[i];
            }
        }
        else
        {
            for (size_t i = 0; i < size; i++)
            {
                mData[i] = sourcePointer[i];
            }
        }
    }

    Integer&
    operator=(int32_t data)
    {
        if (data & (1 << (bits - 1)))
        {
            data |= ~mask;
        }
        else
        {
            data &= mask;
        }

        uint8_t* sourcePointer = reinterpret_cast<uint8_t*>(&data);
        if (isBigEndian())
        {
            for (size_t i = 4 - size; i < 4; i++)
            {
                mData[i - (4 - size)] = sourcePointer[i];
            }
        }
        else
        {
            for (size_t i = 0; i < size; i++)
            {
                mData[i] = sourcePointer[i];
            }
        }
        return *this;
    }

    explicit operator int32_t() const
    {
        int32_t ret = 0;
        uint8_t* targetPointer = reinterpret_cast<uint8_t*>(&ret);
        if (isBigEndian())
        {
            if (static_cast<int8_t>(mData[0]) < 0)
            {
                ret = static_cast<int32_t>(0xFFFFFFFFU);
            }
            for (size_t i = 4 - size; i < 4; i++)
            {
                targetPointer[i] = mData[i - (4 - size)];
            }
        }
        else
        {
            if (static_cast<int8_t>(mData[size - 1]) < 0)
            {
                ret = static_cast<int32_t>(0xFFFFFFFFU);
            }
            for (size_t i = 0; i < size; i++)
            {
                targetPointer[i] = mData[i];
            }
        }
        return ret;
    }

    explicit operator int64_t() const
    {
        return static_cast<PrimitiveType>(*this);
    }

    /**
     * @return the bit width of this type
     */
    static constexpr size_t
    getBits()
    {
        return bits;
    }

    int32_t
    toPrimitiveType() const
    {
        return static_cast<int32_t>(*this);
    }

    bool
    operator==(const Integer& o) const
    {
        for (size_t i = 0; i < size; ++i)
        {
            if (mData[i] != o.mData[i])
            {
                return false;
            }
        }
        return true;
    }

    bool
    operator!=(const Integer& o) const
    {
        return !(*this == o);
    }

    // pre-* versions are enough, intended for the case we want to have them as loop variable

    Integer&
    operator++()
    {
        PrimitiveType value = static_cast<PrimitiveType>(*this);
        ++value;
        *this = value;
        return *this;
    }

    Integer&
    operator--()
    {
        PrimitiveType value = static_cast<PrimitiveType>(*this);
        --value;
        *this = value;
        return *this;
    }

    Integer&
    operator+=(const Integer& o)
    {
        PrimitiveType value = static_cast<PrimitiveType>(*this);
        value += o.toPrimitiveType();
        *this = value;
        return *this;
    }

    Integer&
    operator+=(const PrimitiveType& o)
    {
        PrimitiveType value = static_cast<PrimitiveType>(*this);
        value += o;
        *this = value;
        return *this;
    }

    Integer&
    operator-=(const Integer& o)
    {
        PrimitiveType value = static_cast<PrimitiveType>(*this);
        value -= o.toPrimitiveType();
        *this = value;
        return *this;
    }

    Integer&
    operator-=(const PrimitiveType& o)
    {
        PrimitiveType value = static_cast<PrimitiveType>(*this);
        value -= o;
        *this = value;
        return *this;
    }

    Integer
    operator+(const Integer& o)
    {
        return Integer(toPrimitiveType() + o.toPrimitiveType());
    }

    Integer
    operator-(const Integer& o)
    {
        return Integer(toPrimitiveType() - o.toPrimitiveType());
    }

    Integer
    operator&(const Integer& o) const
    {
        Integer ret;
        for (size_t i = 0; i < size; i++)
        {
            ret.mData[i] = o.mData[i] & mData[i];
        }
        return ret;
    }

    Integer&
    operator&=(const Integer& o)
    {
        for (size_t i = 0; i < size; i++)
        {
            mData[i] = o.mData[i] & mData[i];
        }
        return *this;
    }

    Integer
    operator|(const Integer& o) const
    {
        Integer ret;
        for (size_t i = 0; i < size; i++)
        {
            ret.mData[i] = o.mData[i] | mData[i];
        }
        return ret;
    }

    Integer&
    operator|=(const Integer& o)
    {
        for (size_t i = 0; i < size; i++)
        {
            mData[i] = o.mData[i] | mData[i];
        }
        return *this;
    }

    static constexpr PrimitiveType maxValue =
            static_cast<PrimitiveType>(static_cast<uint32_t>(UINT32_MAX >> (32 - (bits - 1))));
    static constexpr PrimitiveType minValue =
            static_cast<PrimitiveType>(static_cast<uint32_t>(UINT32_MAX & ~maxValue));

private:
    static constexpr size_t size = ((bits - 1) / 8) + 1;
    static constexpr PrimitiveType mask =
            static_cast<PrimitiveType>(static_cast<uint32_t>(UINT32_MAX >> (32 - bits)));
    uint8_t mData[size] = {};
};

template <size_t bits>
class Integer<bits, typename std::enable_if<(bits > 32) && (bits <= 64)>::type>
    : private IntegerBase
{
public:
    using PrimitiveType = int64_t;

    /**
     * For the case you just need an object of the type
     */
    Integer() = default;

    // cppcheck-suppress noExplicitConstructor
    Integer(int64_t data)
    {
        if (data & (static_cast<PrimitiveType>(1L) << (bits - 1)))
        {
            data |= ~mask;
        }
        else
        {
            data &= mask;
        }

        uint8_t* sourcePointer = reinterpret_cast<uint8_t*>(&data);
        if (isBigEndian())
        {
            for (size_t i = 8 - size; i < 8; i++)
            {
                mData[i - (8 - size)] = sourcePointer[i];
            }
        }
        else
        {
            for (size_t i = 0; i < size; i++)
            {
                mData[i] = sourcePointer[i];
            }
        }
    }

    Integer&
    operator=(int64_t data)
    {
        if (data & (1L << (bits - 1)))
        {
            data |= ~mask;
        }
        else
        {
            data &= mask;
        }

        uint8_t* sourcePointer = reinterpret_cast<uint8_t*>(&data);
        if (isBigEndian())
        {
            for (size_t i = 8 - size; i < 8; i++)
            {
                mData[i - (8 - size)] = sourcePointer[i];
            }
        }
        else
        {
            for (size_t i = 0; i < size; i++)
            {
                mData[i] = sourcePointer[i];
            }
        }
        return *this;
    }

    explicit operator int64_t() const
    {
        int64_t ret = 0;
        uint8_t* targetPointer = reinterpret_cast<uint8_t*>(&ret);
        if (isBigEndian())
        {
            if (static_cast<int8_t>(mData[0]) < 0)
            {
                ret = static_cast<int64_t>(0xFFFFFFFFFFFFFFFFLU);
            }
            for (size_t i = 8 - size; i < 8; i++)
            {
                targetPointer[i] = mData[i - (8 - size)];
            }
        }
        else
        {
            if (static_cast<int8_t>(mData[size - 1]) < 0)
            {
                ret = static_cast<int64_t>(0xFFFFFFFFFFFFFFFFLU);
            }
            for (size_t i = 0; i < size; i++)
            {
                targetPointer[i] = mData[i];
            }
        }
        return ret;
    }

    /**
     * @return the bit width of this type
     */
    static constexpr size_t
    getBits()
    {
        return bits;
    }

    int64_t
    toPrimitiveType() const
    {
        return static_cast<int64_t>(*this);
    }

    bool
    operator==(const Integer& o) const
    {
        for (size_t i = 0; i < size; ++i)
        {
            if (mData[i] != o.mData[i])
            {
                return false;
            }
        }
        return true;
    }

    bool
    operator!=(const Integer& o) const
    {
        return !(*this == o);
    }

    // pre-* versions are enough, intended for the case we want to have them as loop variable

    Integer&
    operator++()
    {
        PrimitiveType value = static_cast<PrimitiveType>(*this);
        ++value;
        *this = value;
        return *this;
    }

    Integer&
    operator--()
    {
        PrimitiveType value = static_cast<PrimitiveType>(*this);
        --value;
        *this = value;
        return *this;
    }

    Integer&
    operator+=(const Integer& o)
    {
        PrimitiveType value = static_cast<PrimitiveType>(*this);
        value += o.toPrimitiveType();
        *this = value;
        return *this;
    }

    Integer&
    operator+=(const PrimitiveType& o)
    {
        PrimitiveType value = static_cast<PrimitiveType>(*this);
        value += o;
        *this = value;
        return *this;
    }

    Integer&
    operator-=(const Integer& o)
    {
        PrimitiveType value = static_cast<PrimitiveType>(*this);
        value -= o.toPrimitiveType();
        *this = value;
        return *this;
    }

    Integer&
    operator-=(const PrimitiveType& o)
    {
        PrimitiveType value = static_cast<PrimitiveType>(*this);
        value -= o;
        *this = value;
        return *this;
    }

    Integer
    operator+(const Integer& o)
    {
        return Integer(toPrimitiveType() + o.toPrimitiveType());
    }

    Integer
    operator-(const Integer& o)
    {
        return Integer(toPrimitiveType() - o.toPrimitiveType());
    }

    Integer
    operator&(const Integer& o) const
    {
        Integer ret;
        for (size_t i = 0; i < size; i++)
        {
            ret.mData[i] = o.mData[i] & mData[i];
        }
        return ret;
    }

    Integer&
    operator&=(const Integer& o)
    {
        for (size_t i = 0; i < size; i++)
        {
            mData[i] = o.mData[i] & mData[i];
        }
        return *this;
    }

    Integer
    operator|(const Integer& o) const
    {
        Integer ret;
        for (size_t i = 0; i < size; i++)
        {
            ret.mData[i] = o.mData[i] | mData[i];
        }
        return ret;
    }

    Integer&
    operator|=(const Integer& o)
    {
        for (size_t i = 0; i < size; i++)
        {
            mData[i] = o.mData[i] | mData[i];
        }
        return *this;
    }

    static constexpr PrimitiveType maxValue =
            static_cast<PrimitiveType>(static_cast<uint64_t>(UINT64_MAX >> (64 - (bits - 1))));
    static constexpr PrimitiveType minValue =
            static_cast<PrimitiveType>(static_cast<uint64_t>(UINT64_MAX & ~maxValue));

private:
    static constexpr size_t size = ((bits - 1) / 8) + 1;
    static constexpr PrimitiveType mask =
            static_cast<PrimitiveType>(static_cast<uint64_t>(UINT64_MAX >> (64 - bits)));
    uint8_t mData[size] = {};
};

/*
 * for unsigned ones
 */
template <size_t bits, typename enable = void>
class UInteger
{
};

/**
 * Helper class to check for a type
 */
class UIntegerBase
{
    template <size_t bits, typename enable>
    friend class UInteger;

private:
    /**
     * Ensure only friends can be children
     */
    UIntegerBase() = default;
};

template <size_t bits>
class UInteger<bits, typename std::enable_if<(bits > 0) && (bits <= 8)>::type>
    : private UIntegerBase
{
public:
    using PrimitiveType = uint8_t;

    /**
     * For the case you just need an object of the type
     */
    UInteger() = default;

    // cppcheck-suppress noExplicitConstructor
    UInteger(uint8_t data) : mData(data & mask)
    {
    }

    UInteger
    operator=(uint8_t data)
    {
        mData = data & mask;
        return *this;
    }

    explicit operator uint8_t() const
    {
        return mData;
    }

    explicit operator uint16_t() const
    {
        return mData;
    }

    explicit operator uint32_t() const
    {
        return mData;
    }

    explicit operator uint64_t() const
    {
        return mData;
    }

    explicit operator int16_t() const
    {
        return mData;
    }

    explicit operator int32_t() const
    {
        return mData;
    }

    explicit operator int64_t() const
    {
        return mData;
    }

    /**
     * @return the bit width of this type
     */
    static constexpr size_t
    getBits()
    {
        return bits;
    }

    uint8_t
    toPrimitiveType() const
    {
        return static_cast<uint8_t>(*this);
    }

    bool
    operator==(const UInteger& o) const
    {
        return mData == o.mData;
    }

    bool
    operator!=(const UInteger& o) const
    {
        return !(*this == o);
    }

    UInteger&
    operator++()
    {
        *this = (mData + 1);
        return *this;
    }

    UInteger&
    operator--()
    {
        *this = (mData - 1);
        return *this;
    }

    UInteger&
    operator+=(const UInteger& o)
    {
        *this = mData + o.mData;
        return *this;
    }

    UInteger&
    operator+=(const PrimitiveType& o)
    {
        *this = mData + o;
        return *this;
    }

    UInteger&
    operator-=(const UInteger& o)
    {
        *this = mData - o.mData;
        return *this;
    }

    UInteger&
    operator-=(const PrimitiveType& o)
    {
        *this = mData - o;
        return *this;
    }

    UInteger
    operator+(const UInteger& o)
    {
        return UInteger(mData + o.mData);
    }

    UInteger
    operator-(const UInteger& o)
    {
        return UInteger(mData - o.mData);
    }

    UInteger
    operator&(const UInteger& o) const
    {
        UInteger ret;
        ret.mData = o.mData & mData;
        return ret;
    }

    UInteger&
    operator&=(const UInteger& o)
    {
        mData = o.mData & mData;
        return *this;
    }

    UInteger
    operator|(const UInteger& o) const
    {
        UInteger ret;
        ret.mData = o.mData | mData;
        return ret;
    }

    UInteger&
    operator|=(const UInteger& o)
    {
        mData = o.mData | mData;
        return *this;
    }

    static constexpr PrimitiveType maxValue = static_cast<PrimitiveType>(UINT8_MAX >> (8 - bits));
    static constexpr PrimitiveType minValue = 0;

private:
    static constexpr PrimitiveType mask = maxValue;
    uint8_t mData = 0;
};

template <size_t bits>
class UInteger<bits, typename std::enable_if<(bits > 8) && (bits <= 16)>::type>
    : private UIntegerBase
{
public:
    using PrimitiveType = uint16_t;

    /**
     * For the case you just need an object of the type
     */
    UInteger() = default;

    // cppcheck-suppress noExplicitConstructor
    UInteger(uint16_t data) : mData(data & mask)
    {
    }

    UInteger&
    operator=(uint16_t data)
    {
        mData = data & mask;
        return *this;
    }

    explicit operator uint16_t() const
    {
        return mData;
    }

    explicit operator uint32_t() const
    {
        return mData;
    }

    explicit operator uint64_t() const
    {
        return mData;
    }

    explicit operator int32_t() const
    {
        return mData;
    }

    explicit operator int64_t() const
    {
        return mData;
    }

    /**
     * @return the bit width of this type
     */
    static constexpr size_t
    getBits()
    {
        return bits;
    }

    uint16_t
    toPrimitiveType() const
    {
        return static_cast<uint16_t>(*this);
    }

    bool
    operator==(const UInteger& o) const
    {
        return mData == o.mData;
    }

    bool
    operator!=(const UInteger& o) const
    {
        return !(*this == o);
    }

    UInteger&
    operator++()
    {
        *this = (mData + 1);
        return *this;
    }

    UInteger&
    operator--()
    {
        *this = (mData - 1);
        return *this;
    }

    UInteger&
    operator+=(const UInteger& o)
    {
        *this = mData + o.mData;
        return *this;
    }

    UInteger&
    operator+=(const PrimitiveType& o)
    {
        *this = mData + o;
        return *this;
    }

    UInteger&
    operator-=(const UInteger& o)
    {
        *this = mData - o.mData;
        return *this;
    }

    UInteger&
    operator-=(const PrimitiveType& o)
    {
        *this = mData - o;
        return *this;
    }

    UInteger
    operator+(const UInteger& o)
    {
        return UInteger(mData + o.mData);
    }

    UInteger
    operator-(const UInteger& o)
    {
        return UInteger(mData - o.mData);
    }

    UInteger
    operator&(const UInteger& o) const
    {
        UInteger ret;
        ret.mData = o.mData & mData;
        return ret;
    }

    UInteger&
    operator&=(const UInteger& o)
    {
        mData = o.mData & mData;
        return *this;
    }

    UInteger
    operator|(const UInteger& o) const
    {
        UInteger ret;
        ret.mData = o.mData | mData;
        return ret;
    }

    UInteger&
    operator|=(const UInteger& o)
    {
        mData = o.mData | mData;
        return *this;
    }

    static constexpr PrimitiveType maxValue = static_cast<PrimitiveType>(UINT16_MAX >> (16 - bits));
    static constexpr PrimitiveType minValue = 0;

private:
    static constexpr PrimitiveType mask = maxValue;
    uint16_t mData = 0;
};

template <size_t bits>
class UInteger<bits, typename std::enable_if<(bits > 16) && (bits <= 32)>::type>
    : private UIntegerBase
{
public:
    using PrimitiveType = uint32_t;

    /**
     * For the case you just need an object of the type
     */
    UInteger() = default;

    // cppcheck-suppress noExplicitConstructor
    UInteger(uint32_t data)
    {
        data &= mask;
        uint8_t* sourcePointer = reinterpret_cast<uint8_t*>(&data);
        if (isBigEndian())
        {
            for (size_t i = 4 - size; i < 4; i++)
            {
                mData[i - (4 - size)] = sourcePointer[i];
            }
        }
        else
        {
            for (size_t i = 0; i < size; i++)
            {
                mData[i] = sourcePointer[i];
            }
        }
    }

    UInteger
    operator=(uint32_t data)
    {
        data &= mask;
        uint8_t* sourcePointer = reinterpret_cast<uint8_t*>(&data);
        if (isBigEndian())
        {
            for (size_t i = 4 - size; i < 4; i++)
            {
                mData[i - (4 - size)] = sourcePointer[i];
            }
        }
        else
        {
            for (size_t i = 0; i < size; i++)
            {
                mData[i] = sourcePointer[i];
            }
        }
        return *this;
    }

    explicit operator uint32_t() const
    {
        uint32_t ret = 0;
        uint8_t* targetPointer = reinterpret_cast<uint8_t*>(&ret);
        if (isBigEndian())
        {
            for (size_t i = 4 - size; i < 4; i++)
            {
                targetPointer[i] = mData[i - (4 - size)];
            }
        }
        else
        {
            for (size_t i = 0; i < size; i++)
            {
                targetPointer[i] = mData[i];
            }
        }
        return ret;
    }

    explicit operator uint64_t() const
    {
        return static_cast<PrimitiveType>(*this);
    }

    explicit operator int64_t() const
    {
        return static_cast<PrimitiveType>(*this);
    }

    /**
     * @return the bit width of this type
     */
    static constexpr size_t
    getBits()
    {
        return bits;
    }

    uint32_t
    toPrimitiveType() const
    {
        return static_cast<uint32_t>(*this);
    }

    bool
    operator==(const UInteger& o) const
    {
        for (size_t i = 0; i < size; ++i)
        {
            if (mData[i] != o.mData[i])
            {
                return false;
            }
        }
        return true;
    }

    bool
    operator!=(const UInteger& o) const
    {
        return !(*this == o);
    }

    UInteger&
    operator++()
    {
        PrimitiveType value = static_cast<PrimitiveType>(*this);
        *this = (value + 1);
        return *this;
    }

    UInteger&
    operator--()
    {
        PrimitiveType value = static_cast<PrimitiveType>(*this);
        *this = (value - 1);
        return *this;
    }

    UInteger&
    operator+=(const UInteger& o)
    {
        PrimitiveType value = static_cast<PrimitiveType>(*this);
        value += o.toPrimitiveType();
        *this = value;
        return *this;
    }

    UInteger&
    operator+=(const PrimitiveType& o)
    {
        PrimitiveType value = static_cast<PrimitiveType>(*this);
        value += o;
        *this = value;
        return *this;
    }

    UInteger&
    operator-=(const UInteger& o)
    {
        PrimitiveType value = static_cast<PrimitiveType>(*this);
        value -= o.toPrimitiveType();
        *this = value;
        return *this;
    }

    UInteger&
    operator-=(const PrimitiveType& o)
    {
        PrimitiveType value = static_cast<PrimitiveType>(*this);
        value -= o;
        *this = value;
        return *this;
    }

    UInteger
    operator+(const UInteger& o)
    {
        return UInteger(toPrimitiveType() + o.toPrimitiveType());
    }

    UInteger
    operator-(const UInteger& o)
    {
        return UInteger(toPrimitiveType() - o.toPrimitiveType());
    }

    UInteger
    operator&(const UInteger& o) const
    {
        UInteger ret;
        for (size_t i = 0; i < size; i++)
        {
            ret.mData[i] = o.mData[i] & mData[i];
        }
        return ret;
    }

    UInteger&
    operator&=(const UInteger& o)
    {
        for (size_t i = 0; i < size; i++)
        {
            mData[i] = o.mData[i] & mData[i];
        }
        return *this;
    }

    UInteger
    operator|(const UInteger& o) const
    {
        UInteger ret;
        for (size_t i = 0; i < size; i++)
        {
            ret.mData[i] = o.mData[i] | mData[i];
        }
        return ret;
    }

    UInteger&
    operator|=(const UInteger& o)
    {
        for (size_t i = 0; i < size; i++)
        {
            mData[i] = o.mData[i] | mData[i];
        }
        return *this;
    }

    static constexpr PrimitiveType maxValue = static_cast<PrimitiveType>(UINT32_MAX >> (32 - bits));
    static constexpr PrimitiveType minValue = 0;

private:
    static constexpr size_t size = ((bits - 1) / 8) + 1;
    static constexpr PrimitiveType mask = maxValue;
    uint8_t mData[size] = {};
};

template <size_t bits>
class UInteger<bits, typename std::enable_if<(bits > 32) && (bits <= 64)>::type>
    : private UIntegerBase
{
public:
    using PrimitiveType = uint64_t;

    /**
     * For the case you just need an object of the type
     */
    UInteger() = default;

    // cppcheck-suppress noExplicitConstructor
    UInteger(uint64_t data)
    {
        data &= mask;
        uint8_t* sourcePointer = reinterpret_cast<uint8_t*>(&data);
        if (isBigEndian())
        {
            for (size_t i = 8 - size; i < 8; i++)
            {
                mData[i - (8 - size)] = sourcePointer[i];
            }
        }
        else
        {
            for (size_t i = 0; i < size; i++)
            {
                mData[i] = sourcePointer[i];
            }
        }
    }

    UInteger&
    operator=(uint64_t data)
    {
        data &= mask;
        uint8_t* sourcePointer = reinterpret_cast<uint8_t*>(&data);
        if (isBigEndian())
        {
            for (size_t i = 8 - size; i < 8; i++)
            {
                mData[i - (8 - size)] = sourcePointer[i];
            }
        }
        else
        {
            for (size_t i = 0; i < size; i++)
            {
                mData[i] = sourcePointer[i];
            }
        }
        return *this;
    }

    explicit operator uint64_t() const
    {
        uint64_t ret = 0;
        uint8_t* targetPointer = reinterpret_cast<uint8_t*>(&ret);
        if (isBigEndian())
        {
            for (size_t i = 8 - size; i < 8; i++)
            {
                targetPointer[i] = mData[i - (8 - size)];
            }
        }
        else
        {
            for (size_t i = 0; i < size; i++)
            {
                targetPointer[i] = mData[i];
            }
        }
        return ret;
    }

    /**
     * @return the bit width of this type
     */
    static constexpr size_t
    getBits()
    {
        return bits;
    }

    uint64_t
    toPrimitiveType() const
    {
        return static_cast<uint64_t>(*this);
    }

    bool
    operator==(const UInteger& o) const
    {
        for (size_t i = 0; i < size; ++i)
        {
            if (mData[i] != o.mData[i])
            {
                return false;
            }
        }
        return true;
    }

    bool
    operator!=(const UInteger& o) const
    {
        return !(*this == o);
    }

    UInteger&
    operator++()
    {
        PrimitiveType value = static_cast<PrimitiveType>(*this);
        *this = (value + 1);
        return *this;
    }

    UInteger&
    operator--()
    {
        PrimitiveType value = static_cast<PrimitiveType>(*this);
        *this = (value - 1);
        return *this;
    }

    UInteger&
    operator+=(const UInteger& o)
    {
        PrimitiveType value = static_cast<PrimitiveType>(*this);
        value += o.toPrimitiveType();
        *this = value;
        return *this;
    }

    UInteger&
    operator+=(const PrimitiveType& o)
    {
        PrimitiveType value = static_cast<PrimitiveType>(*this);
        value += o;
        *this = value;
        return *this;
    }

    UInteger&
    operator-=(const UInteger& o)
    {
        PrimitiveType value = static_cast<PrimitiveType>(*this);
        value -= o.toPrimitiveType();
        *this = value;
        return *this;
    }

    UInteger&
    operator-=(const PrimitiveType& o)
    {
        PrimitiveType value = static_cast<PrimitiveType>(*this);
        value -= o;
        *this = value;
        return *this;
    }

    UInteger
    operator+(const UInteger& o)
    {
        return UInteger(toPrimitiveType() + o.toPrimitiveType());
    }

    UInteger
    operator-(const UInteger& o)
    {
        return UInteger(toPrimitiveType() - o.toPrimitiveType());
    }

    UInteger
    operator&(const UInteger& o) const
    {
        UInteger ret;
        for (size_t i = 0; i < size; i++)
        {
            ret.mData[i] = o.mData[i] & mData[i];
        }
        return ret;
    }

    UInteger&
    operator&=(const UInteger& o)
    {
        for (size_t i = 0; i < size; i++)
        {
            mData[i] = o.mData[i] & mData[i];
        }
        return *this;
    }

    UInteger
    operator|(const UInteger& o) const
    {
        UInteger ret;
        for (size_t i = 0; i < size; i++)
        {
            ret.mData[i] = o.mData[i] | mData[i];
        }
        return ret;
    }

    UInteger&
    operator|=(const UInteger& o)
    {
        for (size_t i = 0; i < size; i++)
        {
            mData[i] = o.mData[i] | mData[i];
        }
        return *this;
    }

    static constexpr PrimitiveType maxValue = static_cast<PrimitiveType>(UINT64_MAX >> (64 - bits));
    static constexpr PrimitiveType minValue = 0;

private:
    static constexpr size_t size = ((bits - 1) / 8) + 1;
    static constexpr PrimitiveType mask = maxValue;
    uint8_t mData[size] = {};
};

// ==
template <size_t bits>
inline bool
operator==(const UInteger<bits>& a, const typename UInteger<bits>::PrimitiveType& b)
{
    return a.toPrimitiveType() == b;
}

template <size_t bits>
inline bool
operator==(const typename UInteger<bits>::PrimitiveType& a, const UInteger<bits>& b)
{
    return a == b.toPrimitiveType();
}

template <size_t bits>
inline bool
operator==(const Integer<bits>& a, const typename Integer<bits>::PrimitiveType& b)
{
    return a.toPrimitiveType() == b;
}

template <size_t bits>
inline bool
operator==(const typename Integer<bits>::PrimitiveType& a, const Integer<bits>& b)
{
    return a == b.toPrimitiveType();
}

// !=
template <size_t bits>
inline bool
operator!=(const UInteger<bits>& a, const typename UInteger<bits>::PrimitiveType& b)
{
    return a.toPrimitiveType() != b;
}

template <size_t bits>
inline bool
operator!=(const typename UInteger<bits>::PrimitiveType& a, const UInteger<bits>& b)
{
    return a != b.toPrimitiveType();
}

template <size_t bits>
inline bool
operator!=(const Integer<bits>& a, const typename Integer<bits>::PrimitiveType& b)
{
    return a.toPrimitiveType() != b;
}

template <size_t bits>
inline bool
operator!=(const typename Integer<bits>::PrimitiveType& a, const Integer<bits>& b)
{
    return a != b.toPrimitiveType();
}

// <
template <size_t bits>
inline bool
operator<(const UInteger<bits>& a, const typename UInteger<bits>::PrimitiveType& b)
{
    return a.toPrimitiveType() < b;
}

template <size_t bits>
inline bool
operator<(const typename UInteger<bits>::PrimitiveType& a, const UInteger<bits>& b)
{
    return a < b.toPrimitiveType();
}

template <size_t bits>
inline bool
operator<(const UInteger<bits>& a, const UInteger<bits>& b)
{
    return a.toPrimitiveType() < b.toPrimitiveType();
}

template <size_t bits>
inline bool
operator<(const Integer<bits>& a, const typename Integer<bits>::PrimitiveType& b)
{
    return a.toPrimitiveType() < b;
}

template <size_t bits>
inline bool
operator<(const typename Integer<bits>::PrimitiveType& a, const Integer<bits>& b)
{
    return a < b.toPrimitiveType();
}

template <size_t bits>
inline bool
operator<(const Integer<bits>& a, const Integer<bits>& b)
{
    return a.toPrimitiveType() < b.toPrimitiveType();
}

// <=
template <size_t bits>
inline bool
operator<=(const UInteger<bits>& a, const typename UInteger<bits>::PrimitiveType& b)
{
    return a.toPrimitiveType() <= b;
}

template <size_t bits>
inline bool
operator<=(const typename UInteger<bits>::PrimitiveType& a, const UInteger<bits>& b)
{
    return a <= b.toPrimitiveType();
}

template <size_t bits>
inline bool
operator<=(const UInteger<bits>& a, const UInteger<bits>& b)
{
    return a.toPrimitiveType() <= b.toPrimitiveType();
}

template <size_t bits>
inline bool
operator<=(const Integer<bits>& a, const typename Integer<bits>::PrimitiveType& b)
{
    return a.toPrimitiveType() <= b;
}

template <size_t bits>
inline bool
operator<=(const typename Integer<bits>::PrimitiveType& a, const Integer<bits>& b)
{
    return a <= b.toPrimitiveType();
}

template <size_t bits>
inline bool
operator<=(const Integer<bits>& a, const Integer<bits>& b)
{
    return a.toPrimitiveType() <= b.toPrimitiveType();
}

// >=
template <size_t bits>
inline bool
operator>=(const UInteger<bits>& a, const typename UInteger<bits>::PrimitiveType& b)
{
    return a.toPrimitiveType() >= b;
}

template <size_t bits>
inline bool
operator>=(const typename UInteger<bits>::PrimitiveType& a, const UInteger<bits>& b)
{
    return a >= b.toPrimitiveType();
}

template <size_t bits>
inline bool
operator>=(const UInteger<bits>& a, const UInteger<bits>& b)
{
    return a.toPrimitiveType() >= b.toPrimitiveType();
}

template <size_t bits>
inline bool
operator>=(const Integer<bits>& a, const typename Integer<bits>::PrimitiveType& b)
{
    return a.toPrimitiveType() >= b;
}

template <size_t bits>
inline bool
operator>=(const typename Integer<bits>::PrimitiveType& a, const Integer<bits>& b)
{
    return a >= b.toPrimitiveType();
}

template <size_t bits>
inline bool
operator>=(const Integer<bits>& a, const Integer<bits>& b)
{
    return a.toPrimitiveType() >= b.toPrimitiveType();
}

// >
template <size_t bits>
inline bool
operator>(const UInteger<bits>& a, const typename UInteger<bits>::PrimitiveType& b)
{
    return a.toPrimitiveType() > b;
}

template <size_t bits>
inline bool
operator>(const typename UInteger<bits>::PrimitiveType& a, const UInteger<bits>& b)
{
    return a > b.toPrimitiveType();
}

template <size_t bits>
inline bool
operator>(const UInteger<bits>& a, const UInteger<bits>& b)
{
    return a.toPrimitiveType() > b.toPrimitiveType();
}

template <size_t bits>
inline bool
operator>(const Integer<bits>& a, const typename Integer<bits>::PrimitiveType& b)
{
    return a.toPrimitiveType() > b;
}

template <size_t bits>
inline bool
operator>(const typename Integer<bits>::PrimitiveType& a, const Integer<bits>& b)
{
    return a > b.toPrimitiveType();
}

template <size_t bits>
inline bool
operator>(const Integer<bits>& a, const Integer<bits>& b)
{
    return a.toPrimitiveType() > b.toPrimitiveType();
}

// +
template <size_t bits>
inline typename UInteger<bits>::PrimitiveType
operator+(const UInteger<bits>& a, const typename UInteger<bits>::PrimitiveType& b)
{
    return a.toPrimitiveType() + b;
}

template <size_t bits>
inline typename UInteger<bits>::PrimitiveType
operator+(const typename UInteger<bits>::PrimitiveType& a, const UInteger<bits>& b)
{
    return a + b.toPrimitiveType();
}

template <size_t bits>
inline typename Integer<bits>::PrimitiveType
operator+(const Integer<bits>& a, const typename Integer<bits>::PrimitiveType& b)
{
    return a.toPrimitiveType() + b;
}

template <size_t bits>
inline typename Integer<bits>::PrimitiveType
operator+(const typename Integer<bits>::PrimitiveType& a, const Integer<bits>& b)
{
    return a + b.toPrimitiveType();
}

// -
template <size_t bits>
inline typename UInteger<bits>::PrimitiveType
operator-(const UInteger<bits>& a, const typename UInteger<bits>::PrimitiveType& b)
{
    return a.toPrimitiveType() - b;
}

template <size_t bits>
inline typename UInteger<bits>::PrimitiveType
operator-(const typename UInteger<bits>::PrimitiveType& a, const UInteger<bits>& b)
{
    return a - b.toPrimitiveType();
}

template <size_t bits>
inline typename Integer<bits>::PrimitiveType
operator-(const Integer<bits>& a, const typename Integer<bits>::PrimitiveType& b)
{
    return a.toPrimitiveType() - b;
}

template <size_t bits>
inline typename Integer<bits>::PrimitiveType
operator-(const typename Integer<bits>::PrimitiveType& a, const Integer<bits>& b)
{
    return a - b.toPrimitiveType();
}

// /
template <size_t bits>
inline typename UInteger<bits>::PrimitiveType
operator/(const UInteger<bits>& a, const UInteger<bits>& b)
{
    return a.toPrimitiveType() / b.toPrimitiveType();
}

template <size_t bits>
inline typename UInteger<bits>::PrimitiveType
operator/(const UInteger<bits>& a, const typename UInteger<bits>::PrimitiveType& b)
{
    return a.toPrimitiveType() / b;
}

template <size_t bits>
inline typename UInteger<bits>::PrimitiveType
operator/(const typename UInteger<bits>::PrimitiveType& a, const UInteger<bits>& b)
{
    return a / b.toPrimitiveType();
}

template <size_t bits>
inline typename Integer<bits>::PrimitiveType
operator/(const Integer<bits>& a, const Integer<bits>& b)
{
    return a.toPrimitiveType() / b.toPrimitiveType();
}

template <size_t bits>
inline typename Integer<bits>::PrimitiveType
operator/(const Integer<bits>& a, const typename Integer<bits>::PrimitiveType& b)
{
    return a.toPrimitiveType() / b;
}

template <size_t bits>
inline typename Integer<bits>::PrimitiveType
operator/(const typename Integer<bits>::PrimitiveType& a, const Integer<bits>& b)
{
    return a / b.toPrimitiveType();
}

/**
 * Self defined type trait to check whether a type is a variable width signed integer
 */
template <typename T>
using is_variable_width_signed_integer = std::is_base_of<IntegerBase, T>;

/**
 * Self defined type trait to check whether a type is a variable width unsigned integer
 */
template <typename T>
using is_variable_width_unsigned_integer = std::is_base_of<UIntegerBase, T>;

/**
 * Self defined type trait to check whether a type is any variable width integer
 */
template <typename T>
using is_variable_width_integer =
        typename std::conditional<(is_variable_width_signed_integer<T>::value
                                   || is_variable_width_unsigned_integer<T>::value),
                                  std::true_type,
                                  std::false_type>::type;

}  // namespace utils

using uint24_t = utils::UInteger<24>;
using int24_t = utils::Integer<24>;
using uint48_t = utils::UInteger<48>;
using int48_t = utils::Integer<48>;
}  // namespace outpost

namespace std
{
// PrimitiveType used as return as the constructor cannot be constexpr in C++11.
template <size_t bits>
class numeric_limits<outpost::utils::UInteger<bits>>
{
public:
    static constexpr bool is_specialized = true;
    static constexpr typename outpost::utils::UInteger<bits>::PrimitiveType
    min() noexcept
    {
        return outpost::utils::UInteger<bits>::minValue;
    }
    static constexpr typename outpost::utils::UInteger<bits>::PrimitiveType
    max() noexcept
    {
        return outpost::utils::UInteger<bits>::maxValue;
    }
    static constexpr typename outpost::utils::UInteger<bits>::PrimitiveType
    lowest() noexcept
    {
        return outpost::utils::UInteger<bits>::minValue;
    }
    static constexpr int32_t digits = bits;
    static constexpr int32_t digits10 = bits / 4;  // rough estimate
    static constexpr bool is_signed = false;
    static constexpr bool is_integer = true;
    static constexpr bool is_exact = true;
    static constexpr int32_t radix = 2;
    static constexpr typename outpost::utils::UInteger<bits>::PrimitiveType
    epsilon() noexcept
    {
        return 1;
    }
    static constexpr typename outpost::utils::UInteger<bits>::PrimitiveType
    round_error() noexcept
    {
        return 1;
    }

    static constexpr int32_t min_exponent = 0;
    static constexpr int32_t min_exponent10 = 0;
    static constexpr int32_t max_exponent = 0;
    static constexpr int32_t max_exponent10 = 0;

    static constexpr bool has_infinity = false;
    static constexpr bool has_quiet_NaN = false;
    static constexpr bool has_signaling_NaN = false;
    static constexpr float_denorm_style has_denorm = denorm_absent;
    static constexpr bool has_denorm_loss = false;
    static constexpr typename outpost::utils::UInteger<bits>::PrimitiveType
    infinity() noexcept
    {
        return 0;
    }
    static constexpr typename outpost::utils::UInteger<bits>::PrimitiveType
    quiet_NaN() noexcept
    {
        return 0;
    }
    static constexpr typename outpost::utils::UInteger<bits>::PrimitiveType
    signaling_NaN() noexcept
    {
        return 0;
    }
    static constexpr typename outpost::utils::UInteger<bits>::PrimitiveType
    denorm_min() noexcept
    {
        return 0;
    }

    static constexpr bool is_iec559 = false;
    static constexpr bool is_bounded = true;
    static constexpr bool is_modulo = true;

    static constexpr bool traps = false;
    static constexpr bool tinyness_before = false;
    static constexpr float_round_style round_style = round_toward_zero;
};

// PrimitiveType used as return as the constructor cannot be constexpr in C++11.
template <size_t bits>
class numeric_limits<outpost::utils::Integer<bits>>
{
public:
    static constexpr bool is_specialized = true;
    static constexpr typename outpost::utils::Integer<bits>::PrimitiveType
    min() noexcept
    {
        return outpost::utils::Integer<bits>::minValue;
    }
    static constexpr typename outpost::utils::Integer<bits>::PrimitiveType
    max() noexcept
    {
        return outpost::utils::Integer<bits>::maxValue;
    }
    static constexpr typename outpost::utils::Integer<bits>::PrimitiveType
    lowest() noexcept
    {
        return outpost::utils::Integer<bits>::minValue;
    }
    static constexpr int32_t digits = bits - 1;
    static constexpr int32_t digits10 = (bits - 1) / 4;  // rough estimate
    static constexpr bool is_signed = true;
    static constexpr bool is_integer = true;
    static constexpr bool is_exact = true;
    static constexpr int32_t radix = 2;
    static constexpr typename outpost::utils::Integer<bits>::PrimitiveType
    epsilon() noexcept
    {
        return 1;
    }
    static constexpr typename outpost::utils::Integer<bits>::PrimitiveType
    round_error() noexcept
    {
        return 1;
    }

    static constexpr int32_t min_exponent = 0;
    static constexpr int32_t min_exponent10 = 0;
    static constexpr int32_t max_exponent = 0;
    static constexpr int32_t max_exponent10 = 0;

    static constexpr bool has_infinity = false;
    static constexpr bool has_quiet_NaN = false;
    static constexpr bool has_signaling_NaN = false;
    static constexpr float_denorm_style has_denorm = denorm_absent;
    static constexpr bool has_denorm_loss = false;
    static constexpr typename outpost::utils::Integer<bits>::PrimitiveType
    infinity() noexcept
    {
        return 0;
    }
    static constexpr typename outpost::utils::Integer<bits>::PrimitiveType
    quiet_NaN() noexcept
    {
        return 0;
    }
    static constexpr typename outpost::utils::Integer<bits>::PrimitiveType
    signaling_NaN() noexcept
    {
        return 0;
    }
    static constexpr typename outpost::utils::Integer<bits>::PrimitiveType
    denorm_min() noexcept
    {
        return 0;
    }

    static constexpr bool is_iec559 = false;
    static constexpr bool is_bounded = true;
    static constexpr bool is_modulo = true;

    static constexpr bool traps = false;
    static constexpr bool tinyness_before = false;
    static constexpr float_round_style round_style = round_toward_zero;
};
}  // namespace std

#endif
