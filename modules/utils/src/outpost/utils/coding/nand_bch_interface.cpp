/*
 * Copyright (c) 2019, German Aerospace Center (DLR)
 * All rights reserved.
 *
 * This file is part of the RCN bootloader software.
 * Authors:
 * - 2019, Jan Malburg (DLR RY-AVS)
 */

#include "nand_bch_interface.h"

namespace outpost
{
namespace utils
{
/**
 * Combines the results of 2 Decoding steps into a combined one
 */
DecodeStatus
combine(DecodeStatus a, DecodeStatus b)
{
    // order of ifs are relevant
    if (a == DecodeStatus::invalidParameters || b == DecodeStatus::invalidParameters)
    {
        return DecodeStatus::invalidParameters;
    }
    else if (a == DecodeStatus::uncorrectable || b == DecodeStatus::uncorrectable)
    {
        return DecodeStatus::uncorrectable;
    }
    else if (a == DecodeStatus::corrected || b == DecodeStatus::corrected)
    {
        return DecodeStatus::corrected;
    }
    else
    {
        return DecodeStatus::noError;
    }
}

// default values
constexpr uint32_t NandBCHInterface::DEF_GALIOS_DEMISIONS;
constexpr uint32_t NandBCHInterface::DEF_ERROR_CORRECTION;
constexpr uint8_t NandBCHInterface::fillValue;

}  // namespace utils
}  // namespace outpost
