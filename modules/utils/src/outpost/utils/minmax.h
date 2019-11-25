/*
 * Copyright (c) 2013-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2019, Jan Malburg (DLR RY-AVS)
 */

#ifndef OUTPOST_UTILS_MINMAX_H_
#define OUTPOST_UTILS_MINMAX_H_

namespace outpost
{
namespace utils
{
template <typename return_type, typename A>
inline const return_type
min(const A& value)
{
    return value;
}

template <typename return_type, typename A, typename... Args>
inline return_type
min(const A& a, const Args&... args)
{
    const return_type sub = min<return_type>(args...);
    // cast here so we can be sure that we get min with respect the return type
    if (static_cast<return_type>(a) < sub)
    {
        return a;
    }
    else
    {
        return sub;
    }
}

template <typename return_type, typename A>
inline const return_type
max(const A& value)
{
    return value;
}

template <typename return_type, typename A, typename... Args>
inline return_type
max(const A& a, const Args&... args)
{
    const return_type sub = max<return_type>(args...);
    // cast here so we can be sure that we get max with respect the return type
    // otherwise max<uint8_t>(0x100, 0xff) would be 0x00 for example.
    if (static_cast<return_type>(a) > sub)
    {
        return a;
    }
    else
    {
        return sub;
    }
}

}  // namespace utils
}  // namespace outpost

#endif /* MODULES_UTILS_SRC_OUTPOST_UTILS_MINMAX_H_ */
