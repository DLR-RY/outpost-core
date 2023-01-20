/*
 * Copyright (c) 2020, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2020, Jan-Gerd Mess (DLR RY-AVS)
 * - 2021, Jan Malburg (DLR RY-AVS)
 */

#ifndef OUTPOST_SWB_TESTING_SOFTWARE_BUS_H_
#define OUTPOST_SWB_TESTING_SOFTWARE_BUS_H_

#include <outpost/swb/message_handler.h>

namespace unittest
{
namespace swb
{
class TestingSoftwareBus
{
public:
    explicit TestingSoftwareBus(outpost::swb::MessageHandler& bus);

    bool
    singleMessage();

    void
    allMessages();

private:
    outpost::swb::MessageHandler& mBus;
};

}  // namespace swb
}  // namespace unittest

#endif /* EXT_OUTPOST_CORE_MODULES_SWB_TEST_OUTPOST_SWB_TESTING_SOFTWARE_BUS_H_ */
