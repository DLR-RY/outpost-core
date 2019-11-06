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
/**
 * Fixpoint numbers for fast arithmetic operations
 *
 * \tparam  PREC
 *      Precision, i.e. the number of  fractional bits to use.
 *
 * \author  Jan-Gerd Mess
 */
template <unsigned PREC>
class FP
{
public:
    template <unsigned PP>
    friend class FP;

    // Constructors from various standard types
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

    /**
     * Absolute value of the given number.
     * \return Positive number with the same value
     */
    FP
    abs() const
    {
        if (value < 0)
        {
            return FP(-value);
        }
        return FP(value);
    }

    /**
     * Explicit cast to int32_t including rounding.
     *
     * \return Rounded int32_t
     */
    explicit operator int32_t()
    {
        uint8_t round = 0;

        if (((1 << (PREC - 1) & abs().getValue()) && value > 0)
            || (!(1 << (PREC - 1) & abs().getValue()) && value < 0))
        {
            round = 1;
        }
        return static_cast<int32_t>(value >> PREC) + round;
    }

    /**
     * Explicit cast to int16_t including rounding.
     *
     * \return Rounded int16_t
     */
    explicit operator int16_t() const
    {
        uint8_t round = 0;
        if (((1 << (PREC - 1) & abs().getValue()) && value > 0)
            || (!(1 << (PREC - 1) & abs().getValue()) && value < 0))
        {
            round = 1;
        }
        return static_cast<int16_t>(value >> PREC) + round;
    }

    /**
     * Explicit cast to float
     *
     * \return Value as float
     */
    explicit operator float() const
    {
        return (static_cast<float>(value)) / (1 << PREC);
    }

    /**
     * Explicit cast to double
     *
     * \return Value as double
     */
    explicit operator double() const
    {
        return (static_cast<double>(value)) / (1 << PREC);
    }

    /**
     * Assignment operator from int32_t
     * \param x Assigned value
     * \return Value as fixpoint number
     */
    FP&
    operator=(const int32_t& x)
    {
        value = x;
        return *this;
    }

    /**
     * Assignment operator from int16_t
     * \param x Assigned value
     * \return Value as fixpoint number
     */
    FP&
    operator=(const int16_t& x)
    {
        value = static_cast<int32_t>(x * (1 << PREC));
        return *this;
    }

    /**
     * Assignment operator from float
     * \param x Assigned value
     * \return Value as fixpoint number
     */
    FP&
    operator=(const float& x)
    {
        value = static_cast<int32_t>(x * (1 << PREC));
        return *this;
    }

    /**
     * Assignment operator from double
     * \param x Assigned value
     * \return Value as fixpoint number
     */
    FP&
    operator=(const double& x)
    {
        value = static_cast<int32_t>(x * (1 << PREC));
        return *this;
    }

    /**
     * Assignment operator for other fixpoint numbers
     * \param x Assigned value
     * \return Fixpoint number
     */
    FP&
    operator=(const FP& x)
    {
        value = x.value;
        return *this;
    }

    /**
     * Add assignment operator for fixpoint numbers
     * \param x Summand
     * \return Sum of the current and a second fixpoint number
     */
    FP&
    operator+=(const FP& x)
    {
        value += x.value;
        return *this;
    }

    /**
     * Add operator for fixpoint numbers
     * \param x Summand
     * \return Sum of the current and a second fixpoint number
     */
    FP
    operator+(const FP& x) const
    {
        FP res(*this);
        return res += x;
    }

    /**
     * Add assignment operator for arbitrary types. Casts to FP first.
     * \param x Summand
     * \return Sum of the current and the casted input number
     */
    template <typename TT>
    FP&
    operator+=(const TT& x)
    {
        *this += FP(x);
        return (*this);
    }

    /**
     * Add operator for arbitrary types. Casts to FP first.
     * \param x Summand
     * \return Sum of the current and the casted input number
     */
    template <typename TT>
    FP
    operator+(const TT& x) const
    {
        FP res(*this);
        return res += FP(x);
    }

    /**
     * Subtraction assignment operator for fixpoint numbers
     * \param x Subtrahend
     * \return Difference of the current and a second fixpoint number
     */
    FP&
    operator-=(const FP& x)
    {
        value -= x.value;
        return *this;
    }

    /**
     * Subtraction operator for arbitrary types. Casts to FP first.
     * \param x Subtrahend
     * \return Difference of the current and the casted input number
     */
    FP
    operator-(const FP& x) const
    {
        FP res(*this);
        return res -= x;
    }

    /**
     * Subtract assignment operator for arbitrary types. Casts to FP first.
     * \param x Subtrahend
     * \return Difference of the current and the casted input number
     */
    template <typename TT>
    FP&
    operator-=(const TT& x)
    {
        *this -= FP(x);
        return (*this);
    }

    /**
     * Subtract operator for arbitrary types. Casts to FP first.
     * \param x Subtrahend
     * \return Difference of the current and the casted input number
     */
    template <typename TT>
    FP
    operator-(const TT& x) const
    {
        FP res(*this);
        return res -= FP(x);
    }

    /**
     * Multiplication assignment operator for fixpoint numbers
     * \param x Factor
     * \return Product of the current and a second fixpoint number
     */
    FP&
    operator*=(const FP& x)
    {
        typedef long long int T2;
        value = static_cast<int32_t>((static_cast<T2>(value) * static_cast<T2>(x.value)) >> PREC);
        return *this;
    }

    /**
     * Multiplication operator for fixpoint numbers
     * \param x Factor
     * \return Product of the current and a second fixpoint number
     */
    FP operator*(const FP& x) const
    {
        FP res(*this);
        return res *= x;
    }

    /**
     * Multiplication assignment operator for arbitrarily typed numbers
     * \param x Factor
     * \return Product of the current and the casted input number
     */
    template <typename TT>
    FP&
    operator*=(const TT& x)
    {
        *this *= FP(x);
        return (*this);
    }

    /**
     * Multiplication operator for arbitrarily typed numbers
     * \param x Factor
     * \return Product of the current and the casted input number
     */
    template <typename TT>
    FP operator*(const TT& x) const
    {
        FP res(*this);
        return res *= FP(x);
    }

    /**
     * Division assignment operator for fixpoint numbers
     * \param x Divisor
     * \return Quotient of the current and a second fixpoint number
     */
    FP&
    operator/=(const FP& x)
    {
        value = static_cast<int32_t>(((static_cast<int64_t>(value)) << PREC)
                                     / (static_cast<int64_t>(x.value)));
        return *this;
    }

    /**
     * Division operator for fixpoint numbers
     * \param x Divisor
     * \return Quotient of the current and a second fixpoint number
     */
    FP
    operator/(const FP& x) const
    {
        FP res(*this);
        return res /= x;
    }

    /**
     * Division assignment operator for arbitrarily typed numbers
     * \param x Divisor
     * \return Quotient of the current and the casted input number
     */
    template <typename TT>
    FP&
    operator/=(const TT& x)
    {
        *this /= FP(x);
        return (*this);
    }

    /**
     * Division operator for arbitrarily typed numbers
     * \param x Divisor
     * \return Quotient of the current and the casted input number
     */
    template <typename TT>
    FP
    operator/(const TT& x) const
    {
        FP res(*this);
        return res /= FP(x);
    }

    /**
     * Shift left assignment operator
     * \param x Number of bits to shift
     * \return Fixpoint shifted by x bits
     */
    FP&
    operator<<=(const unsigned x)
    {
        value <<= x;
        return *this;
    }

    /**
     * Shift left operator
     * \param x Number of bits to shift
     * \return Fixpoint shifted by x bits
     */
    FP
    operator<<(const unsigned x) const
    {
        FP res(*this);
        return res <<= x;
    }

    /**
     * Shift right assignment operator
     * \param x Number of bits to shift
     * \return Fixpoint shifted by x bits
     */
    FP&
    operator>>=(const unsigned x)
    {
        value >>= x;
        return *this;
    }

    /**
     * Shift right operator
     * \param x Number of bits to shift
     * \return Fixpoint shifted by x bits
     */
    FP
    operator>>(const unsigned x) const
    {
        FP res(*this);
        return res >>= x;
    }

    /**
     * Less than operator for arbitrary types. Casts to FP first.
     * \param x Number to compare to
     * \return True iff the current value is less than x
     */
    template <typename TT>
    bool
    operator<(const TT& x) const
    {
        return (*this < FP(x));
    }

    /**
     * Less than operator for other fixpoint numbers.
     * \param x Number to compare to
     * \return True iff the current value is less than x
     */
    bool
    operator<(const FP& x) const
    {
        return (value < x.value);
    }

    /**
     * Greater than operator for arbitrary types. Casts to FP first.
     * \param x Number to compare to
     * \return True iff the current value is greater than x
     */
    template <typename TT>
    bool
    operator>(const TT& x) const
    {
        return (*this > FP(x));
    }

    /**
     * Greater than operator for other fixpoint numbers.
     * \param x Number to compare to
     * \return True iff the current value is greater than x
     */
    bool
    operator>(const FP& x) const
    {
        return (value > x.value);
    }

    /**
     * Equality operator for arbitrary types. Casts to FP first.
     * \param x Number to compare to
     * \return True iff the current value is equal to x
     */
    template <typename TT>
    bool
    operator==(const TT& x) const
    {
        return (*this == FP(x));
    }

    /**
     * Equality operator for other fixpoint numbers.
     * \param x Number to compare to
     * \return True iff the current value is equal to x
     */
    bool
    operator==(const FP& x) const
    {
        return value == x.value;
    }

    /**
     * Inequality operator for arbitrary types. Casts to FP first.
     * \param x Number to compare to
     * \return True iff the current value is not equal to x
     */
    template <typename TT>
    bool
    operator!=(const TT& x) const
    {
        return (*this != FP(x));
    }

    /**
     * Inequality operator for other fixpoint numbers.
     * \param x Number to compare to
     * \return True iff the current value is not equal to x
     */
    bool
    operator!=(const FP& x) const
    {
        return value != x.value;
    }

    /**
     * Less than or equal to operator for arbitrary types. Casts to FP first.
     * \param x Number to compare to
     * \return True iff the current value is less than or equal to x
     */
    template <typename TT>
    bool
    operator<=(const TT& x) const
    {
        return (*this <= FP(x));
    }

    /**
     * Less than or equal to operator for other fixpoint numbers.
     * \param x Number to compare to
     * \return True iff the current value is less than or equal to x.
     */
    bool
    operator<=(const FP& x) const
    {
        return value <= x.value;
    }

    /**
     * Greater than or equal to operator for arbitrary types. Casts to FP first.
     * \param x Number to compare to
     * \return True iff the current value is greater than or equal to x
     */
    template <typename TT>
    bool
    operator>=(const TT& x) const
    {
        return (*this >= FP(x));
    }

    /**
     * Greater than or equal to operator for other fixpoint numbers.
     * \param x Number to compare to
     * \return True iff the current value is greater than or equal to x
     */
    bool
    operator>=(const FP& x) const
    {
        return value >= x.value;
    }

    /**
     * Getter the raw underlying int32_tv value.
     * \return Raw underlying value
     */
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
