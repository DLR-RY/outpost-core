/*
 * Copyright (c) 2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#ifndef OUTPOST_UTILS_SERIALIZABLE_OBJECT_H
#define OUTPOST_UTILS_SERIALIZABLE_OBJECT_H

#include "serialize.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

namespace outpost
{
/**
 * Abstract base class for objects than can be serialized.
 *
 * \author  Fabian Greif
 */
class SerializableObject
{
public:
    virtual ~SerializableObject();

    /**
     * Get the size of the parameter.
     *
     * \return  Size of the parameter in bytes.
     */
    virtual size_t
    getSerializedSize() const = 0;

    /**
     * Write parameter value onto the output stream.
     *
     * \param stream
     *      Output stream
     */
    virtual void
    serialize(Serialize& stream) const = 0;

    /**
     * Read parameter value from stream.
     *
     * \param stream
     *      Input stream
     * \retval  true    Parameter could be read and validated.
     * \retval  false   Parameter validation failed.
     */
    virtual bool
    deserialize(Deserialize& stream) = 0;
};

}  // namespace outpost

#endif
