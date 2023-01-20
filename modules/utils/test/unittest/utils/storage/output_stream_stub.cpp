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

#include "output_stream_stub.h"

#include <stdio.h>

size_t
unittest::utils::OutputStreamStub::append(const outpost::Slice<const uint8_t>& data)
{
    if (mData.size() >= mLimit)
    {
        return 0;
    }
    else if (mData.size() + data.getNumberOfElements() <= mLimit)
    {
        mData.insert(mData.end(), data.begin(), data.end());
        return data.getNumberOfElements();
    }
    else
    {
        auto tmp = data.first(mLimit - mData.size());
        mData.insert(mData.end(), tmp.begin(), tmp.end());
        return tmp.getNumberOfElements();
    }
}
