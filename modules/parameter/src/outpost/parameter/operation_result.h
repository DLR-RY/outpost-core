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
 * - 2020, Jan Malburg (DLR RY-AVS)
 */

#ifndef OUTPOST_PARAMETER_OPERATIONRESULT_H_
#define OUTPOST_PARAMETER_OPERATIONRESULT_H_

namespace outpost
{
namespace parameter
{
enum class OperationResult
{
    success,                  // everything is fine
    incorrectType,            // requested type that was not correct
    concurrentWrite,          // write failed due to another write currently in progress
    notInitialized,           // Function called before object initialized
    alreadyInitialized,       // tried to initialized more than once
    invalidParameter,         // Parameters are not valid
    tooManyConcurrentWrites,  // read failed because there were at least maxReadTries writes during
                              // execution
    noSuchID,                 // no Parameter exists for the requested ID
    dublicatedID,             // two or more Parameter in the list have identical IDs
    uninitializedParameter,   // at least one Parameter was not initialized
    tooManyElements           // the list contains more than maxElements Parameter
};
}
}  // namespace outpost

#endif
