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
 * - 2021, Jan Malburg (DLR RY-AVS)
 */

#ifndef OUTPOST_SWB_MESSAGE_HANDLER_H_
#define OUTPOST_SWB_MESSAGE_HANDLER_H_

namespace unittest
{
namespace swb
{
class TestingSoftwareBus;
}
}  // namespace unittest

namespace outpost
{
namespace swb
{
/**
 * For unit testing, class allows to handle single or all messages
 * queued to the bus distributer without needing to start the thread;
 * hence preventing race condition in the unit test.
 *
 * Putting this as its own file to prevent relations to other header files,
 * especially from default/outpost/parameter/support.h as this is not in
 * the unit test path and different projects may have different version.
 */
class MessageHandler
{
    friend class unittest::swb::TestingSoftwareBus;

protected:
    MessageHandler() = default;
    virtual ~MessageHandler() = default;

    /**
     * Will handle all messages queued,
     * use in case no running thread is wanted in order to
     * remove race conditions
     */
    virtual void
    handleAllMessages() = 0;

    /**
     * Will handle a single message if queued
     * @return true if a message was handled, false otherwise
     */
    virtual bool
    handleSingleMessage() = 0;
};

}  // namespace swb
}  // namespace outpost

#endif /* MODULES_SWB_SRC_OUTPOST_SWB_MESSAGE_HANDLER_H_ */
