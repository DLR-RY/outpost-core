/*
 * Copyright (c) 2019, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2019, Jan Malburg (DLR RY-AVS)
 */

#if __cplusplus < 201402L
#error "Compile time NandBCh encoder/decoder needs at least C++14"
#else

#ifndef NAND_BCH_COMPILETIME_H_
#define NAND_BCH_COMPILETIME_H_

#include "../pow.h"
#include "nand_bch_interface.h"

#include <stdint.h>

namespace outpost
{
namespace utils
{
/**
 * Class to encode/decode nand pages with BCH error correction.
 * Warning: Functions not thread-safe add mutexes or us different instances of class.
 */
template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
class NandBCHCTime : public NandBCHInterface
{
    static constexpr uint32_t mNumDataBytes = 512;

    static_assert(mTParam >= 4, "Minimal value for mTParam is 4");
    static_assert((mNandDataSize % mNumDataBytes) == 0, "mNumDataBytes shall be multiple of 512");
    static_assert(mMParam >= 2, "Minimal supported value for mMParam = 2");
    static_assert(mMParam <= 15, "Maximal supported value for mMParam =  15");

public:
    NandBCHCTime(void);

    bool
    encode(const outpost::Slice<const uint8_t>& src_data,
           const outpost::Slice<uint8_t>& coded_data) override;

    DecodeStatus
    decode(const outpost::Slice<const uint8_t>& coded_data,
           const outpost::Slice<uint8_t>& src_data) override;

    inline uint32_t
    getNumberOfRedundantBytes(void) const override
    {
        return mNumRedundantBytes * (mNandDataSize / mNumDataBytes);
    }

    inline uint32_t
    getNumberOfDatabytes(void) const override
    {
        return mNandDataSize;
    }

    inline bool
    isTemplateParameterValid(void) const override
    {
        // In the compile time variant this is checked via static_assert
        return mValid;
    }

    bool
    isChecksumEmpty(const outpost::Slice<const uint8_t>& data) override;

private:
    struct ALogTable;
    struct LogTable;
    struct QuadCompTable;
    struct Polynom;
    struct EncodeTable;

    static constexpr ALogTable
    buildALogTables(void);

    static constexpr LogTable
    buildLogTables(void);

    static constexpr QuadCompTable
    genQuadCompTable(void);

    static constexpr int32_t
    getTraceTestVal(void);

    static constexpr bool
    checkLogTables(void);

    constexpr static int32_t
    ffMult(int32_t a, int32_t b);

    constexpr static int32_t
    ffInv(int32_t opa, bool& success);

    constexpr static int32_t
    ffDiv(int32_t opa, int32_t opb, bool& success);

    constexpr static int32_t
    ffSquareRoot(int32_t opa);

    constexpr static int32_t
    ffCubeRoot(int32_t opa, bool& success);

    constexpr int32_t
    ffQuadFun(int32_t c);

    static constexpr Polynom
    generateCodeGenPoly(void);

    static constexpr EncodeTable
    generateEncodeTables(void);

    static constexpr void
    convertGenPolyBitToWord(Polynom& poly, const uint32_t* genPolyBitArray);

    static constexpr uint32_t fieldPolyTable[22] = {0x1,  // dummy value
                                                    0x1,  // dummy value
                                                    0x5,
                                                    0xb,
                                                    0x13,
                                                    0x25,
                                                    0x43,
                                                    0x83,
                                                    0x11d,
                                                    0x211,
                                                    0x409,
                                                    0x805,
                                                    0x1053,
                                                    0x201b,
                                                    0x402b,
                                                    0x8003};

    static constexpr uint32_t mFFPoly = fieldPolyTable[mMParam];

    // Using default code configuration
    static constexpr uint32_t mFFSize = outpost::PowerOfTwo<mMParam>::value;
    static constexpr uint32_t mMOddParam = mMParam % 2;
    static constexpr uint32_t mNParam = mFFSize - 1;
    static constexpr uint32_t mLogZVal = 2 * mNParam;
    static constexpr uint32_t mNumDataBits = mNumDataBytes * 8;

    static constexpr ALogTable aLogTable = buildALogTables();
    static constexpr LogTable logTable = buildLogTables();
    static constexpr int32_t mTraceTestVal = getTraceTestVal();
    static constexpr QuadCompTable mQuadCompTable = genQuadCompTable();

    static constexpr uint32_t BYTESTATES = 256;

    static constexpr uint32_t ZERO_DIV_RETURN_VALUE = 1;

    static constexpr uint32_t numSyndromes = 2 * mTParam;

    uint32_t mLoc[(2 * mTParam) + 1];
    uint32_t mSyndromes[numSyndromes];
    uint16_t mErrLocByte[mTParam];
    uint8_t mErrLocBit[mTParam];
    bool mValid;

    void
    bchEncode(void);

    int32_t
    computeRemainder(void);

    void
    computeSyndromes(void);

    uint32_t
    berMas(int32_t sigmaN[], bool& success);

    bool
    chienSearch(int32_t sigmaN[], const int32_t LnOrig);

    bool
    quadraticElp(const int32_t sigmaN[]);

    bool
    cubicElp(const int32_t sigmaN[]);

    bool
    quarticElp(int32_t sigmaN[]);

    bool
    rootFindChien(int32_t sigmaN[], const int32_t LnOrig);

    bool
    fixErrors(int32_t Ln);

    DecodeStatus
    bchDecode(void);

    struct QuadCompTable
    {
        constexpr QuadCompTable(void) : table{} {};

        int32_t table[mMParam];

        constexpr const int32_t& operator[](int i) const
        {
            return table[i];
        };
        int32_t& operator[](int i)
        {
            return table[i];
        };
    };

    struct LogTable
    {
        constexpr LogTable(void) : table{} {};
        uint16_t table[mFFSize];

        constexpr const uint16_t& operator[](int i) const
        {
            return table[i];
        };
        uint16_t& operator[](int i)
        {
            return table[i];
        };
    };

    struct ALogTable
    {
        constexpr ALogTable(void) : table{} {};
        uint16_t table[2 * mFFSize];

        constexpr const uint16_t& operator[](int i) const
        {
            return table[i];
        };
        uint16_t& operator[](int i)
        {
            return table[i];
        };
    };

    struct Polynom
    {
        constexpr Polynom() : genPolyFdbkWords{}, genPolyDegree(0), valid(false){};
        uint32_t genPolyFdbkWords[(((mTParam * mMParam) / 8 + 1) / 4 + 1)];
        uint32_t genPolyDegree;
        bool valid;
    };

    static constexpr Polynom mPolynom = generateCodeGenPoly();

    static constexpr const uint32_t* const genPolyFdbkWords = mPolynom.genPolyFdbkWords;
    static constexpr uint32_t genPolyDegree = mPolynom.genPolyDegree;
    static constexpr uint32_t mNumRedundantBits = genPolyDegree;
    static constexpr uint32_t mNumRedundantBytes =
            ((genPolyDegree % 8) > 0) ? (genPolyDegree / 8) + 1 : (genPolyDegree / 8);

    // Note: number of data length must be less than equal to (mNParam - genPolyDegree) / 8
    // value in our case it is 1010 bytes
    static constexpr uint32_t mNumCodeWordBytes = mNumDataBytes + mNumRedundantBytes;
    uint8_t mCodeWord[mNumCodeWordBytes];
    uint8_t mRemainderBytes[mNumRedundantBytes];

    static constexpr uint32_t mNumRedundantWords = ((mNumRedundantBytes % 4) > 0)
                                                           ? (mNumRedundantBytes / 4) + 1
                                                           : (mNumRedundantBytes / 4);

    struct EncodeTable
    {
        constexpr EncodeTable() : encodeTable{} {};
        uint32_t encodeTable[BYTESTATES * mNumRedundantWords];

        struct Column
        {
            constexpr Column() : p(0){};

            uint32_t* p;

            constexpr const uint32_t& operator[](int i) const
            {
                return p[i];
            };

            constexpr uint32_t& operator[](int i)
            {
                return p[i];
            };
        };

        struct ConstColumn
        {
            constexpr ConstColumn() : p(0){};

            const uint32_t* p;

            constexpr const uint32_t& operator[](int i) const
            {
                return p[i];
            };
        };

        constexpr const ConstColumn operator[](int i) const
        {
            ConstColumn c;
            c.p = &encodeTable[i * mNumRedundantWords];
            return c;
        };

        constexpr Column operator[](int i)
        {
            Column c;
            c.p = &encodeTable[i * mNumRedundantWords];
            return c;
        };
    };

    static constexpr EncodeTable encodeTable = generateEncodeTables();
};

}  // namespace utils
}  // namespace outpost

#include "nand_bch_compiletime_impl.h"
#endif /* NAND_BCH_H_ */

#endif  // version check
