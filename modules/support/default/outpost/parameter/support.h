/*
 * Copyright (c) 2018, German Aerospace Center (DLR)
 * All rights reserved.
 *
 * This file is part of the development version of OUTPOST.
 *
 * May contain parts that are subject to export control under
 * the terms of AL Nr.: 9D005 (EG Nr. 428/2009 Dual Use) for
 * export to countries outside the EU.
 *
 * Authors:
 * - 2018, Annika Ofenloch (DLR RY-AVS)
 */

#ifndef OUTPOST_SUPPORT_PARAMETER_SUPPORT_H
#define OUTPOST_SUPPORT_PARAMETER_SUPPORT_H

#include <stdint.h>

namespace outpost
{
namespace support
{
namespace parameter
{

enum class HeartbeatSource : uint8_t
{
    default0 = 0,
    default1 = 1,
    lastId
};

} // namespace parameter
} // namespace support
} // namespace outpost

#endif
