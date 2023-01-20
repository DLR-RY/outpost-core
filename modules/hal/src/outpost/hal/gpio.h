/*
 * Copyright (c) 2021, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2021, Andre Nahrwold (DLR SC-SRV)
 */

#ifndef OUTPOST_HAL_GPIO_H
#define OUTPOST_HAL_GPIO_H

#include <outpost/base/callable.h>

#include <limits>
#include <stdint.h>

#include <cstddef>
#include <type_traits>

namespace outpost
{
namespace hal
{
/**
 * Gpi Interface.
 *
 * The default value of the mask parameters provides a more intuitive interface
 * for single gpi pins.
 *
 * \author Andre Nahrwold
 */
template <typename BankType = uint32_t>
class Gpi
{
    static_assert(std::is_unsigned<BankType>::value, "BankType must be unsigned integral type");

public:
    /**
     * Source:
     * https://devel.rtems.org/wiki/GSoC/2013/Raspberry_Pi_BSP_Peripherals#Usingdigitalinputs
     */
    enum class InterruptType
    {
        none,

        fallingEdge,
        risingEdge,
        bothEdges,

        lowLevel,
        highLevel,
        bothLevels
    };

    enum class ReturnCode
    {
        success,

        interruptAlreadyConfigured,
        invalidInterruptType,
        invalidObject,
        invalidHandler,
        invalidMask,
        interruptNotConfigured,

        failure
    };

    /**
     * Type of the gpi interrupt handler function.
     *
     * \param type
     *      Informs the handler which edge or level triggered it.
     * \param mask
     *      The pins which triggered it
     */
    typedef void (Callable::*Handler)(InterruptType type, BankType mask);

public:
    static constexpr BankType maskAllSet = std::numeric_limits<BankType>::max();

public:
    Gpi() = default;

    virtual ~Gpi() = default;

    /**
     * Get the current state of the gpi pins.
     *
     * \param mask
     *      The mask to select the gpi pin(s).
     * \return BankType The current state of the gpi pins.
     *      "0" represents a logic zero, "1" a logic one.
     */
    virtual BankType
    get(BankType mask = maskAllSet) = 0;

    /**
     * Configure the interrupt functionality for one or more gpi pins.
     *
     * \warning The handler might be called multiple times for one instance,
     *          but only once per pin when e.g. the both edges interrupt type is
     *          selected. Also it is only possible to configure one handler
     *          per instance.
     *
     * \param type
     *      The type for which the interrupt handler shall be called.
     * \param object
     *      The callable object instance which contains the handler function.
     * \param handler
     *      The interrupt handler to be called.
     * \param mask
     *      The mask to select the gpi pin(s).
     *
     * \return
     *      An interface specific return code
     */
    virtual ReturnCode
    configureInterrupt(InterruptType type,
                       Callable* object,
                       Handler handler,
                       BankType mask = maskAllSet) = 0;

    /**
     * Enable the interrupt functionality for one or more gpi pins.
     *
     * \return
     *      An interface specific return code
     */
    virtual ReturnCode
    enableInterrupt() = 0;

    /**
     * Disable the interrupt functionality for one or more gpi pins.
     *
     * \return
     *      An interface specific return code
     */
    virtual ReturnCode
    disableInterrupt() = 0;
};

/**
 * Gpo Interface.
 *
 * The default value of the mask parameters provides a more intuitive interface
 * for single gpo pins.
 *
 * \author Andre Nahrwold
 */
template <typename BankType = uint32_t>
class Gpo
{
    static_assert(std::is_unsigned<BankType>::value, "BankType must be unsigned integral type");

public:
    enum class ReturnCode
    {
        success,

        invalidMask,

        failure
    };

public:
    static constexpr BankType maskAllSet = std::numeric_limits<BankType>::max();

public:
    Gpo() = default;

    virtual ~Gpo() = default;

    /**
     * Set one or more gpo pins to logic one.
     *
     * \param mask
     *      The mask to select the gpo pin(s).
     *
     * \return
     *      An interface specific return code
     */
    virtual ReturnCode
    set(BankType mask = maskAllSet) = 0;

    /**
     * Clear one or more gpo pins to logic zero.
     *
     * \param mask
     *      The mask to select the gpo pin(s).
     *
     * \return
     *      An interface specific return code
     */
    virtual ReturnCode
    clear(BankType mask = maskAllSet) = 0;

    /**
     * Toggle one or more gpo pins.
     *
     * \param mask
     *      The mask to select the gpo pin(s).
     *
     * \return
     *      An interface specific return code
     */
    virtual ReturnCode
    toggle(BankType mask = maskAllSet) = 0;

    /**
     * Get the current state of the gpo pins.
     *
     * \param mask
     *      The mask to select the gpo pin(s).
     * \return BankType The current state of the gpo pins.
     *      "0" represents a logic zero, "1" a logic one.
     */
    virtual BankType
    get(BankType mask = maskAllSet) = 0;
};

}  // namespace hal
}  // namespace outpost

#endif
