/*
 * Copyright (c) 2013-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2017-2019, Jan-Gerd Mess (DLR RY-AVS)
 */

#ifndef OUTPOST_BASE_FIXPOINT_H_
#define OUTPOST_BASE_FIXPOINT_H_

#include <stdint.h>
#include <stdio.h>

namespace outpost
{
template <unsigned PREC>
class FP
{
public:
    template <unsigned PP>
    friend class FP;

    FP() : value(0)
    {
    }

    FP(const FP& x) : value(x.value)
    {
    }

    FP(int16_t x) : value(x * (1 << PREC))
    {
    }

    FP(int32_t x) : value(x)
    {
    }

    FP(float x) : value(x * (1 << PREC))
    {
    }

    FP(double x) : value(x * (1 << PREC))
    {
    }

    FP
	abs() const
	{
		if (value < 0)
		{
			return FP(-value);
		}
		return FP(value);
	}

    explicit operator int32_t()
    {
    	uint8_t round = 0;

        if (
            ((1 << (PREC - 1) & abs().getValue()) && value > 0) ||
			(!(1 << (PREC - 1) & abs().getValue()) && value < 0))
        {
                round = 1;
        }
        return static_cast<int32_t>(value >> PREC) + round;
    }

    explicit operator int16_t() const
    {
    	uint8_t round = 0;
    	if (
			((1 << (PREC - 1) & abs().getValue()) && value > 0) ||
			(!(1 << (PREC - 1) & abs().getValue()) && value < 0))
		{
				round = 1;
		}
		return static_cast<int16_t>(value >> PREC) + round;
    }

    explicit operator float() const
    {
        return (static_cast<float>(value)) / (1 << PREC);
    }

    explicit operator double() const
    {
        return (static_cast<double>(value)) / (1 << PREC);
    }

    FP&
    operator=(const int32_t& x)
    {
        value = x;
        return *this;
    }

    FP&
    operator=(const int16_t& x)
    {
        value = static_cast<int32_t>(x * (1 << PREC));
        return *this;
    }

    FP&
    operator=(const float& x)
    {
        value = static_cast<int32_t>(x * (1 << PREC));
        return *this;
    }

    FP&
    operator=(const double& x)
    {
        value = static_cast<int32_t>(x * (1 << PREC));
        return *this;
    }

    FP&
    operator=(const FP& x)
    {
        value = x.value;
        return *this;
    }

    FP&
    operator+=(const FP& x)
    {
        value += x.value;
        return *this;
    }

    FP
    operator+(const FP& x) const
    {
        FP res(*this);
        return res += x;
    }

    template <typename TT>
    FP&
    operator+=(const TT& x)
    {
        *this += FP(x);
        return (*this);
    }

    template <typename TT>
    FP
    operator+(const TT& x) const
    {
        FP res(*this);
        return res += FP(x);
    }


    FP&
    operator-=(const FP& x)
    {
        value -= x.value;  // check
        return *this;
    }

    FP
    operator-(const FP& x) const
    {
        FP res(*this);
        return res -= x;
    }

    template <typename TT>
    FP&
    operator-=(const TT& x)
    {
        *this -= FP(x);
        return (*this);
    }

    template <typename TT>
    FP
    operator-(const TT& x) const
    {
        FP res(*this);
        return res -= FP(x);
    }

    FP&
    operator*=(const FP& x)
    {
        typedef long long int T2;
        value = static_cast<int32_t>((static_cast<T2>(value) * static_cast<T2>(x.value)) >> PREC);
        return *this;
    }
    /** Multiplication */
    FP operator*(const FP& x) const
    {
        FP res(*this);
        return res *= x;
    }

    template <typename TT>
    FP&
    operator*=(const TT& x)
    {
        *this *= FP(x);
        return (*this);
    }

    template <typename TT>
    FP operator*(const TT& x) const
    {
        FP res(*this);
        return res *= FP(x);
    }

    FP&
    operator/=(const FP& x)
    {
        typedef long long int T2;
        value = static_cast<int32_t>(((static_cast<T2>(value)) << PREC)
                                     / (static_cast<T2>(x.value)));
        return *this;
    }

    FP
    operator/(const FP& x) const
    {
        FP res(*this);
        return res /= x;
    }

    FP&
    operator<<=(const unsigned x)
    {
        value <<= x;
        return *this;
    }

    FP
    operator<<(const unsigned x) const
    {
        FP res(*this);
        return res <<= x;
    }

    FP&
    operator>>=(const unsigned x)
    {
        value >>= x;
        return *this;
    }

    FP
    operator>>(const unsigned x) const
    {
        FP res(*this);
        return res >>= x;
    }

    template <typename TT>
    bool
    operator<(const TT& x) const
    {
        return (*this < FP(x));
    }
    bool
    operator<(const FP& x) const
    {
        return (value < x.value);
    }

    template <typename TT>
    bool
    operator>(const TT& x) const
    {
        return (*this > FP(x));
    }
    bool
    operator>(const FP& x) const
    {
        return (value > x.value);
    }
    /** Equal */
    bool
    operator==(const FP& x) const
    {
        return value == x.value;
    }
    bool
    operator!=(const FP& x) const
    {
        return value != x.value;
    }
    bool
    operator<=(const FP& x) const
    {
        return value <= x.value;
    }
    bool
    operator>=(const FP& x) const
    {
        return value >= x.value;
    }

    int32_t
    getValue()
    {
        return value;
    }

private:
    int32_t value;
};

typedef FP<16> Fixpoint;

}  // namespace outpost

#endif /* OUTPOST_BASE_FIXPOINT_H_ */
