/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * 
 * This file is part of libCOBC 0.4.
 *
 * It is distributed under the terms of the GNU General Public License with a
 * linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */
// ----------------------------------------------------------------------------

#ifndef COBC_SMPC_SUBSCRIBER_H
#define COBC_SMPC_SUBSCRIBER_H

namespace cobc
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
