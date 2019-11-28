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

#include "protocol_dispatcher_thread.h"

namespace outpost
{
namespace hal
{
void
ProtocolDispatcherThread::run()
{
    while (true)
    {
        outpost::support::Heartbeat::send(mHeartbeatSource, mWaitTime + mDispatchTime);

        // ensures receive does not change the mBuffer length.
        outpost::Slice<uint8_t> tmp = mBuffer;
        uint32_t readByte = mReceiver.receive(tmp, mWaitTime);
        if (readByte > 0)
        {
            mPD.handlePackage(mBuffer, readByte);
        }
    }
}

}  // namespace hal
}  // namespace outpost
