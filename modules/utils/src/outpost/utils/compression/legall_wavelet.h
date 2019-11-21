/*
 * Copyright (c) 2015-2019, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2015-2019, Jan-Gerd Mess (DLR RY-AVS)
 */

#ifndef OUTPOST_UTILS_COMPRESSION_LEGALL_WAVELET_H
#define OUTPOST_UTILS_COMPRESSION_LEGALL_WAVELET_H

#include <outpost/base/fixpoint.h>
#include <outpost/utils/log2.h>

namespace outpost
{
namespace compression
{
/**
 * Implementation of the Le Gall 5/3 wavelet
 * For further information, check:
 * https://ieeexplore.ieee.org/document/157221
 *
 * For the complete compression scheme, see:
 * https://elib.dlr.de/112826/
 */
class LeGall53Wavelet
{
public:
    /**
     * Forward transformation
     * @param inBuffer
     *     Pointer to an array of FP<16> that shall be transformed to wavelet coefficients.
     * @param bufferLength
     *     Number of elements in inBuffer, which also equals the resulting number of elements in
     * outBuffer
     * @param outBuffer
     *     Pointer to store the resulting transformed data. Needs to be able to store bufferLength
     * elements.
     */
    static void forwardTransform(FP<16>* inBuffer, FP<16>* outBuffer, size_t bufferLength);

    /**
     * Forward transformation in place, requiring only one buffer but with a different ordering of
     * coefficients.
     * @param inBuffer
     *     Pointer to an array of FP<16> that shall be transformed to wavelet coefficients.
     * @param length
     *     Number of elements in inBuffer
     */
    static void forwardTransformInPlace(FP<16>* inBuffer, size_t length);

    /**
     * Reorders the coefficients after in place transformation for further coding by using the bits
     * after the comma.
     */
    static void reorder(FP<16>* inBuffer, size_t inBufferLength);

    /**
     * Backward transformation for ground use (i.e. using floating point numbers)
     * @param inBuffer
     *     Pointer to an array of double that shall be transformed back to timeseries values.
     * @param bufferLength
     *     Number of elements in inBuffer, which also equals the resulting number of elements in
     * outBuffer
     * @param outBuffer
     *     Pointer to store the resulting transformed data. Needs to be able to store bufferLength
     * elements.
     */
    static void
    backwardTransform(double* inBuffer, double* outBuffer, size_t bufferLength);

private:
    // Forward lowpass coefficients
    static const FP<16> h0;
    static const FP<16> h1;
    static const FP<16> h2;
    static const FP<16> h3;
    static const FP<16> h4;

    // Forward highpass coefficients
    static const FP<16> g0;
    static const FP<16> g1;
    static const FP<16> g2;

    // Forward highpass coefficients for in place calculations using the lifting scheme
    static const FP<16> ip_g0;
    static const FP<16> ip_g1;
    static const FP<16> ip_g2;
    static const FP<16> ip_g3;
    static const FP<16> ip_g4;

    // Backward (inverse) lowpass coefficients
    static const double ih0;
    static const double ih1;
    static const double ih2;

    // Backward (inverse) highpass coefficients
    static const double ig0;
    static const double ig1;
    static const double ig2;
    static const double ig3;
    static const double ig4;

    LeGall53Wavelet() = delete;
    ~LeGall53Wavelet() = delete;
};

}  // namespace compression

}  // namespace outpost
#endif  // OUTPOST_UTILS_COMPRESSION_LEGALL_WAVELET_H
