/*
 * Copyright (c) 2013-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2013-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#ifndef COBC_SMPC_SUBSCRIBER_H
#define COBC_SMPC_SUBSCRIBER_H

namespace outpost
{
namespace smpc
{

/**
 * Empty base class for every class that should receive data from
 * a topic.
 *
 * This class is needed to provide a common base class to cast to when
 * using the member functions in the Subscriber class.
 *
 * \see Subscriber
 * \see Topic
 *
 * \ingroup smpc
 * \author  Fabian Greif
 */
class Subscriber
{
};

}
}

#endif // COBC_SMPC_SUBSCRIBER_H
