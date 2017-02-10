/*
 * Copyright (c) 2016, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.6.
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

#include <unittest/harness.h>

#include <cobc/hal/register.h>

struct TestRegister
{
    static const uint32_t baseAddress = 0xE0000000;

    struct General
    {
        static const uint32_t offset = 0x00;
        static const uint32_t address = baseAddress + offset;

        typedef cobc::hal::Register::Bitfield<uint32_t, 31, 0, address> All;

        typedef cobc::hal::Register::Bitfield<uint32_t, 31, 24, address> Prescaler;
        typedef cobc::hal::Register::SingleBit<uint32_t, 23, address> Polarity;
        typedef cobc::hal::Register::SingleBit<uint32_t, 16, address> Enable;
        typedef cobc::hal::Register::SingleBit<uint32_t, 4, address> Bypass;
        typedef cobc::hal::Register::SingleBit<uint32_t, 2, address> EncoderEnable;
        typedef cobc::hal::Register::SingleBit<uint32_t, 1, address> RandomizerEnable;
        typedef cobc::hal::Register::SingleBit<uint32_t, 0, address> DecoderEnable;
    };
};

using cobc::hal::Register;

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
        value = Register::getValue<TestRegister::General::Prescaler>(100) |
                Register::getValue<TestRegister::General::Enable>(1);
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
    volatile uint32_t* output = Register::getPointer<volatile uint32_t>(TestRegister::General::address + 4*sizeof(uint32_t));
    EXPECT_EQ(expected, output);
}
