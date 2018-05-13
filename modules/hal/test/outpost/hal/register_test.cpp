/*
 * Copyright (c) 2016-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2016-2017, Fabian Greif (DLR RY-AVS)
 */

#include <outpost/hal/register.h>

#include <unittest/harness.h>

struct TestRegister
{
    static const uint32_t baseAddress = 0xE0000000;

    struct General
    {
        static const uint32_t offset = 0x00;
        static const uint32_t address = baseAddress + offset;

        typedef outpost::hal::Register::Bitfield<uint32_t, 31, 0, address> All;

        typedef outpost::hal::Register::Bitfield<uint32_t, 31, 24, address> Prescaler;
        typedef outpost::hal::Register::SingleBit<uint32_t, 23, address> Polarity;
        typedef outpost::hal::Register::SingleBit<uint32_t, 16, address> Enable;
        typedef outpost::hal::Register::SingleBit<uint32_t, 4, address> Bypass;
        typedef outpost::hal::Register::SingleBit<uint32_t, 2, address> EncoderEnable;
        typedef outpost::hal::Register::SingleBit<uint32_t, 1, address> RandomizerEnable;
        typedef outpost::hal::Register::SingleBit<uint32_t, 0, address> DecoderEnable;
    };
};

using outpost::hal::Register;

TEST(RegisterTest, shouldCompile)
{
    // The templates write to specific hardware address defined at compile-time.
    // This makes it hard to test them reliable in form of unit-tests.
    //
    // Nonetheless the test here verifies that it is possible to instantiate
    // the templates as intended.
    if (false)
    {
        // Direct read and write access
        uint32_t value = Register::read<TestRegister::General::Prescaler>();
        Register::write<TestRegister::General::Prescaler>(100);

        // Writing multiple values in one access
        value = Register::getValue<TestRegister::General::Prescaler>(100)
                | Register::getValue<TestRegister::General::Enable>(1);
        Register::write<TestRegister::General::All>(value);

        // Bit operator access
        value = Register::read<TestRegister::General::All>();
        value &= ~Register::getMask<TestRegister::General::Enable>();
        value |= Register::getValue<TestRegister::General::Enable>(1);
        Register::write<TestRegister::General::All>(value);

        // Read modify write for two values
        value = Register::read<TestRegister::General::All>();
        Register::writeToMemory<TestRegister::General::Prescaler>(100, value);
        Register::writeToMemory<TestRegister::General::Enable>(1, value);
        Register::write<TestRegister::General::All>(value);

        // Read complete value to memory and extract needed bits
        value = Register::read<TestRegister::General::All>();
    }
}

TEST(RegisterTest, shouldAccessMemory)
{
    uint32_t value = 0;
    Register::writeToMemory<TestRegister::General::Prescaler>(100, value);
    Register::writeToMemory<TestRegister::General::Enable>(1, value);
    EXPECT_EQ(0x64010000U, value);

    uint32_t prescaler = Register::readFromMemory<TestRegister::General::Prescaler>(value);
    EXPECT_EQ(100U, prescaler);

    Register::writeToMemory<TestRegister::General::All>(100, value);
    EXPECT_EQ(100U, value);
}

TEST(RegisterTest, shouldCastToPointer)
{
    volatile uint32_t* expected = reinterpret_cast<volatile uint32_t*>(0xE0000010);
    volatile uint32_t* output = Register::getPointer<volatile uint32_t>(
            TestRegister::General::address + 4 * sizeof(uint32_t));
    EXPECT_EQ(expected, output);
}
