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

#ifndef UNITTEST_UTILS_STORAGE_OUTPUT_STREAM_STUB_H_
#define UNITTEST_UTILS_STORAGE_OUTPUT_STREAM_STUB_H_

#include <outpost/utils/storage/output_stream.h>

#include <vector>

namespace unittest
{
namespace utils
{
class OutputStreamStub : public outpost::utils::OutputStream
{
public:
    OutputStreamStub() = default;

    size_t
    append(const outpost::Slice<const uint8_t>& data) override;

    /**
     * @return all appended data
     */
    std::vector<uint8_t>&
    getData()
    {
        return mData;
    }

    /**
     * Set a limit of how many elements at most to append
     * @param limit the new limit
     */
    void
    setLimit(size_t limit)
    {
        mLimit = limit;
    }

    void
    clear()
    {
        mData.clear();
    }

private:
    std::vector<uint8_t> mData;
    size_t mLimit = SIZE_MAX;
};
}  // namespace utils
}  // namespace unittest

#endif
