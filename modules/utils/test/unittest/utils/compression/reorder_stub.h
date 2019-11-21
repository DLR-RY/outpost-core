/*
 * reorder_stub.h
 *
 *  Created on: Dec 15, 2017
 *      Author: user
 */

#ifndef MODULES_COMPRESSION_TEST_UNITTEST_REORDER_STUB_H_
#define MODULES_COMPRESSION_TEST_UNITTEST_REORDER_STUB_H_

#include <outpost/base/fixpoint.h>

namespace unittest
{
namespace compression
{
class ReorderStub
{
public:
    ReorderStub();

    static void
    reorder(outpost::Fixpoint* inBuffer, size_t inBufferLength);

    ~ReorderStub();
};

} /* namespace compression */
}  // namespace unittest

#endif /* MODULES_COMPRESSION_TEST_UNITTEST_REORDER_STUB_H_ */
