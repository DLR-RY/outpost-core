/*
 * Copyright (c) 2019, German Aerospace Center (DLR)
 * All rights reserved.
 *
 * This file is part of the RCN bootloader software.
 * Authors:
 * - 2019, Jan Malburg (DLR RY-AVS)
 */

#ifndef COMMON_MODULES_NAND_MIL_NAND_BCH_INTERFACE_H_
#define COMMON_MODULES_NAND_MIL_NAND_BCH_INTERFACE_H_

#include <outpost/base/slice.h>

#include <stdint.h>

namespace outpost
{
namespace utils
{
class NandBCHInterface
{
public:
    // default values
    static constexpr uint32_t DEF_GALIOS_DEMISIONS =
            13;  // Dimension of Galois Field supported up to 15
    static constexpr uint32_t DEF_ERROR_CORRECTION =
            8;  // Default error correction power in bits supported up to 20
    static constexpr uint8_t fillValue =
            0x00;  // value to fill up if less data then nand page data size s provided

    NandBCHInterface() = default;

    virtual ~NandBCHInterface() = default;

    virtual bool
    isChecksumEmpty(uint8_t* buffer) = 0;

    virtual bool
    encode(outpost::Slice<const uint8_t> src_data, outpost::Slice<uint8_t> coded_data) = 0;

    virtual bool
    decode(outpost::Slice<const uint8_t> coded_data, outpost::Slice<uint8_t> src_data) = 0;

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
