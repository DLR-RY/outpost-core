/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
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
public:
    inline
    Subscriber()
    {
    }

    inline
    ~Subscriber()
    {
    }

    inline
    Subscriber(const Subscriber&)
    {
    }

    inline
    Subscriber&
    operator =(const Subscriber&)
    {
        return *this;
    }
};

}
}

#endif // COBC_SMPC_SUBSCRIBER_H
