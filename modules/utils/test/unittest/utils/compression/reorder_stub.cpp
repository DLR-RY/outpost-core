/*
 * reorder_stub.cpp
 *
 *  Created on: Dec 15, 2017
 *      Author: user
 */

#include "reorder_stub.h"

namespace unittest
{
namespace compression
{
ReorderStub::ReorderStub()
{
    // TODO Auto-generated constructor stub
}

void ReorderStub::reorder(outpost::FP<16>* inBuffer, size_t inBufferLength)
{
    int16_t* outputBuffer = reinterpret_cast<int16_t*>(inBuffer);
    size_t index = 2;
    outputBuffer[0] = static_cast<int16_t>(inBuffer[0]);
    for (size_t step = inBufferLength >> 1; step >= 1; step >>= 1)
    {
        for (size_t i = step; i < inBufferLength; i += 2 * step)
        {
            outputBuffer[index] = static_cast<int16_t>(inBuffer[i]);
            index += 2;
        }
    }

    for (size_t i = 0; i < inBufferLength; i++)
    {
        outputBuffer[i] = outputBuffer[2 * i];
    }
}

ReorderStub::~ReorderStub()
{
    // TODO Auto-generated destructor stub
}

} /* namespace compression */
}  // namespace unittest
