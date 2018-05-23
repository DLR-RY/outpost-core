/*
 * Copyright (c) 2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2017, Jan-Gerd Mess (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#ifndef OUTPOST_UTILS_SMART_BUFFER_IMPL_H_
#define OUTPOST_UTILS_SMART_BUFFER_IMPL_H_

#include "smart_buffer.h"

namespace outpost
{
namespace utils
{
template <size_t N>
SmartBuffer::SmartBuffer(uint8_t (&array)[N]) :
    mReferenceCounter(0),
    mBuffer(array),
    mNumElements(N)
{
}

}  // namespace utils
}  // namespace outpost

#endif
