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
// default values
constexpr uint32_t NandBCHInterface::DEF_GALIOS_DEMISIONS;
constexpr uint32_t NandBCHInterface::DEF_ERROR_CORRECTION;
constexpr uint8_t NandBCHInterface::fillValue;

}  // namespace utils
}  // namespace outpost
