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
    if ((a == DecodeStatus::invalidParameters) || (b == DecodeStatus::invalidParameters))
    {
        return DecodeStatus::invalidParameters;
    }
    else if ((a == DecodeStatus::uncorrectable) || (b == DecodeStatus::uncorrectable))
    {
        return DecodeStatus::uncorrectable;
    }
    else if ((a == DecodeStatus::corrected) || (b == DecodeStatus::corrected))
    {
        return DecodeStatus::corrected;
    }
    else
    {
        return DecodeStatus::noError;
    }
}

// default values
constexpr uint32_t NandBCHInterface::DEF_GALIOS_DIMENISIONS;
constexpr uint32_t NandBCHInterface::DEF_ERROR_CORRECTION;
constexpr uint8_t NandBCHInterface::fillValue;

}  // namespace utils
}  // namespace outpost
