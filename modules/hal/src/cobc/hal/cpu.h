/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------
#ifndef CPU_HAL_H_
#define CPU_HAL_H_

namespace cobc
{
namespace hal
{
/**
 * CPU Interface
 *
 * \author  Norbert Toth
 */
class Cpu
{
public:

    virtual
    ~Cpu() = 0;

    virtual void
    reset() = 0;

};
}
}

#endif // CPU_HAL_H_
