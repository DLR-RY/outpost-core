/*
 * Copyright (c) 2021, German Aerospace Center (DLR)
 * All rights reserved.
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2021, Jan Malburg (DLR RY-AVS)
 */

#ifndef OUTPOST_UTILS_STORAGE_OUTPUT_STREAM_H_
#define OUTPOST_UTILS_STORAGE_OUTPUT_STREAM_H_

#include <outpost/base/slice.h>

#include <stdint.h>

namespace outpost
{
namespace utils
{
class OutputStream
{
public:
    OutputStream() = default;
    virtual ~OutputStream() = default;

    /**
     * Pushes data into the stream
     *
     * @param data data to push into the stream
     * @return number of bytes successfully pushed into the stream,
     *         operation successful if return value == data.getNumberOfElements()
     */
    virtual size_t
    append(const outpost::Slice<const uint8_t>& data) = 0;
};
}  // namespace utils
}  // namespace outpost

#endif
