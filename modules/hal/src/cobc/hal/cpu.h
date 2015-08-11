/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
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
