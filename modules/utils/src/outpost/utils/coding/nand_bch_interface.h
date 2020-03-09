/*
 * Copyright (c) 2015 - 2020, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2015, Muhammad Bassam (DLR RY-AVS)
 * - 2019 - 2020, Jan Malburg (DLR RY-AVS)
 */

#ifndef COMMON_MODULES_NAND_MIL_NAND_BCH_INTERFACE_H_
#define COMMON_MODULES_NAND_MIL_NAND_BCH_INTERFACE_H_

#include <outpost/base/slice.h>

#include <stdint.h>

namespace outpost
{
namespace utils
{
enum class DecodeStatus
{
    invalidParameters,
    noError,
    corrected,
    uncorrectable
};

/**
 * Combines the results of 2 Decoding steps into a combined one
 */
DecodeStatus
combine(DecodeStatus a, DecodeStatus b);

class NandBCHInterface
{
public:
    // default values
    static constexpr uint32_t DEF_GALIOS_DIMENISIONS = 13;  // Dimension of Galois Field
    static constexpr uint32_t DEF_ERROR_CORRECTION = 8;  // Default error correction power in bits
    static constexpr uint8_t fillValue =
            0x00;  // value to fill up if less data then nand page data size s provided

    NandBCHInterface() = default;

    virtual ~NandBCHInterface() = default;

    virtual bool
    isChecksumEmpty(const outpost::Slice<const uint8_t>& data) = 0;

    virtual bool
    encode(const outpost::Slice<const uint8_t>& src_data,
           const outpost::Slice<uint8_t>& coded_data) = 0;

    virtual DecodeStatus
    decode(const outpost::Slice<const uint8_t>& coded_data,
           const outpost::Slice<uint8_t>& src_data) = 0;

    virtual uint32_t
    getNumberOfRedundantBytes(void) const = 0;

    virtual uint32_t
    getNumberOfDatabytes(void) const = 0;

    virtual bool
    isTemplateParameterValid(void) const = 0;
};

}  // namespace utils
}  // namespace outpost

#endif /* COMMON_MODULES_NAND_MIL_NAND_BCH_INTERFACE_H_ */
