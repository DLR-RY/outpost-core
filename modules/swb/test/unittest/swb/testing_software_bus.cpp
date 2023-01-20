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

#include "testing_software_bus.h"

using namespace unittest::swb;
using namespace outpost::swb;

TestingSoftwareBus::TestingSoftwareBus(outpost::swb::MessageHandler& bus) : mBus(bus)
{
}

bool
TestingSoftwareBus::singleMessage()
{
    return mBus.handleSingleMessage();
}

void
TestingSoftwareBus::allMessages()
{
    mBus.handleAllMessages();
}
