/*
 * Copyright (c) 2020, German Aerospace Center (DLR)
 * All rights reserved.
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2020, Jan Malburg (DLR RY-AVS)
 */

#ifndef OUTPOST_UTILS_STORAGE_BIG_ENDIAN_INT_H
#define OUTPOST_UTILS_STORAGE_BIG_ENDIAN_INT_H

#include <stdint.h>

namespace outpost
{
namespace utils
{
/*
 * A set of classes to store data in defined endian format
 * For efficiency reason everything is inline.
 */

inline bool
isBigEndian()
{
    uint32_t tmp = 0x00112233;
    // cppcheck will think this is always true but it ain't
    // cppcheck-suppress knownConditionTrueFalse
    return *reinterpret_cast<uint8_t*>(&tmp) == 0x00;
}

class __attribute__((packed)) beint64
{
public:
    inline beint64() : value(0)
    {
    }

    inline beint64(int64_t v) : value(isBigEndian() ? v : turn(v))
    {
    }

    inline beint64(const beint64& o) : value(o.value)
    {
    }

    inline operator int64_t() const
    {
        if (isBigEndian())
        {
            return value;
        }
        else
        {
            return turn(value);
        }
    }

    inline beint64&
    operator=(const int64_t v)
    {
        if (isBigEndian())
        {
            value = v;
        }
        else
        {
            value = turn(v);
        }
        return *this;
    }

    inline beint64&
    operator=(const beint64 v)
    {
        value = v.value;
        return *this;
    }

private:
    int64_t value = 0;

    static inline int64_t
    turn(const int64_t v)
    {
        int64_t ret;
        uint8_t* target = reinterpret_cast<uint8_t*>(&ret);
        const uint8_t* source = reinterpret_cast<const uint8_t*>(&v);
        target[0] = source[7];
        target[1] = source[6];
        target[2] = source[5];
        target[3] = source[4];
        target[4] = source[3];
        target[5] = source[2];
        target[6] = source[1];
        target[7] = source[0];
        return ret;
    }
};

class __attribute__((packed)) beint32
{
public:
    inline beint32() : value(0)
    {
    }

    inline beint32(int32_t v) : value(isBigEndian() ? v : turn(v))
    {
    }

    inline beint32(const beint32& o) : value(o.value)
    {
    }

    inline operator int32_t() const
    {
        if (isBigEndian())
        {
            return value;
        }
        else
        {
            return turn(value);
        }
    }

    inline beint32&
    operator=(const int32_t v)
    {
        if (isBigEndian())
        {
            value = v;
        }
        else
        {
            value = turn(v);
        }
        return *this;
    }

    inline beint32&
    operator=(const beint32 v)
    {
        value = v.value;
        return *this;
    }

private:
    int32_t value = 0;

    static inline int32_t
    turn(const int32_t v)
    {
        int32_t ret;
        uint8_t* target = reinterpret_cast<uint8_t*>(&ret);
        const uint8_t* source = reinterpret_cast<const uint8_t*>(&v);
        target[0] = source[3];
        target[1] = source[2];
        target[2] = source[1];
        target[3] = source[0];
        return ret;
    }
};

class __attribute__((packed)) beint16
{
public:
    inline beint16() : value(0)
    {
    }

    inline beint16(uint16_t v) : value(isBigEndian() ? v : turn(v))
    {
    }

    inline beint16(const beint16& o) : value(o.value)
    {
    }

    inline operator int16_t() const
    {
        if (isBigEndian())
        {
            return value;
        }
        else
        {
            return turn(value);
        }
    }

    inline beint16&
    operator=(const int16_t v)
    {
        if (isBigEndian())
        {
            value = v;
        }
        else
        {
            value = turn(v);
        }
        return *this;
    }

    inline beint16&
    operator=(const beint16 v)
    {
        value = v.value;
        return *this;
    }

private:
    int16_t value = 0;

    static inline int16_t
    turn(const int16_t v)
    {
        int16_t ret;
        uint8_t* target = reinterpret_cast<uint8_t*>(&ret);
        const uint8_t* source = reinterpret_cast<const uint8_t*>(&v);
        target[0] = source[1];
        target[1] = source[0];
        return ret;
    }
};

class __attribute__((packed)) beuint64
{
public:
    inline beuint64() : value(0)
    {
    }

    inline beuint64(uint64_t v) : value(isBigEndian() ? v : turn(v))
    {
    }

    inline beuint64(const beuint64& o) : value(o.value)
    {
    }

    inline operator uint64_t() const
    {
        if (isBigEndian())
        {
            return value;
        }
        else
        {
            return turn(value);
        }
    }

    inline beuint64&
    operator=(const uint64_t v)
    {
        if (isBigEndian())
        {
            value = v;
        }
        else
        {
            value = turn(v);
        }
        return *this;
    }

    inline beuint64&
    operator=(const beuint64 v)
    {
        value = v.value;
        return *this;
    }

private:
    uint64_t value = 0;

    static inline uint64_t
    turn(const uint64_t v)
    {
        uint64_t ret;
        uint8_t* target = reinterpret_cast<uint8_t*>(&ret);
        const uint8_t* source = reinterpret_cast<const uint8_t*>(&v);
        target[0] = source[7];
        target[1] = source[6];
        target[2] = source[5];
        target[3] = source[4];
        target[4] = source[3];
        target[5] = source[2];
        target[6] = source[1];
        target[7] = source[0];
        return ret;
    }
};

class __attribute__((packed)) beuint32
{
public:
    inline beuint32() : value(0)
    {
    }

    inline beuint32(uint32_t v) : value(isBigEndian() ? v : turn(v))
    {
    }

    inline beuint32(const beuint32& o) : value(o.value)
    {
    }

    inline operator uint32_t() const
    {
        if (isBigEndian())
        {
            return value;
        }
        else
        {
            return turn(value);
        }
    }

    inline beuint32&
    operator=(const uint32_t v)
    {
        if (isBigEndian())
        {
            value = v;
        }
        else
        {
            value = turn(v);
        }
        return *this;
    }

    inline beuint32&
    operator=(const beuint32 v)
    {
        value = v.value;
        return *this;
    }

private:
    uint32_t value = 0;

    static inline uint32_t
    turn(const uint32_t v)
    {
        uint32_t ret;
        uint8_t* target = reinterpret_cast<uint8_t*>(&ret);
        const uint8_t* source = reinterpret_cast<const uint8_t*>(&v);
        target[0] = source[3];
        target[1] = source[2];
        target[2] = source[1];
        target[3] = source[0];
        return ret;
    }
};

class __attribute__((packed)) beuint16
{
public:
    inline beuint16() : value(0)
    {
    }

    inline beuint16(uint16_t v) : value(isBigEndian() ? v : turn(v))
    {
    }

    inline beuint16(const beuint16& o) : value(o.value)
    {
    }

    inline operator uint16_t() const
    {
        if (isBigEndian())
        {
            return value;
        }
        else
        {
            return turn(value);
        }
    }

    inline beuint16&
    operator=(const uint16_t v)
    {
        if (isBigEndian())
        {
            value = v;
        }
        else
        {
            value = turn(v);
        }
        return *this;
    }

    inline beuint16&
    operator=(const beuint16 v)
    {
        value = v.value;
        return *this;
    }

private:
    uint16_t value = 0;

    static inline uint16_t
    turn(const uint16_t v)
    {
        uint16_t ret;
        uint8_t* target = reinterpret_cast<uint8_t*>(&ret);
        const uint8_t* source = reinterpret_cast<const uint8_t*>(&v);
        target[0] = source[1];
        target[1] = source[0];
        return ret;
    }
};

}  // namespace utils
}  // namespace outpost

#endif
