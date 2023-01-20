/*
 * Copyright (c) 2015 - 2020, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2015, Muhammad Bassam (DLR RY-AVS)
 * - 2019 - 2020, Jan Malburg (DLR RY-AVS)
 */

#ifndef NAND_BCH_RUNTIME_H_
#define NAND_BCH_RUNTIME_H_

#include "../pow.h"
#include "nand_bch_interface.h"

#include <stdint.h>

#include <bitset>

namespace outpost
{
namespace utils
{
/**
 * Class to encode/decode nand pages with BCH error correction.
 * Warning: Functions not thread-safe add mutexes or us different instances of class.
 *
 * Note: For use different instance  compile time version is suggested as this (with default values)
 *       only requires 704 bytes per instance, compared to this class with 98656 bytes per instance
 *       (for default values)
 */
template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
class NandBCHRTime : public NandBCHInterface
{
    static constexpr uint32_t mNumDataBytes = 512;
    static constexpr uint32_t mNumDataBits = mNumDataBytes * 8;

    static_assert(mTParam >= 4, "Min Supported value for mTParam is 4");
    static_assert((mNandDataSize % mNumDataBytes) == 0, "mNandDataSize shall be multiple of 512");
    static_assert(mMParam >= 2, "Minimal supported value for mMParam = 2");
    static_assert(mMParam <= 15, "Maximal supported value for mMParam =  15");

public:
    NandBCHRTime(void);

    bool
    encode(const outpost::Slice<const uint8_t>& src_data,
           const outpost::Slice<uint8_t>& coded_data) override;

    DecodeStatus
    decode(const outpost::Slice<const uint8_t>& coded_data,
           const outpost::Slice<uint8_t>& dest_data) override;

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
        return mValid;
    }

    bool
    isChecksumEmpty(const outpost::Slice<const uint8_t>& data) override;

private:
    static constexpr uint32_t mFFSize = outpost::PowerOfTwo<mMParam>::value;
    static constexpr uint32_t MAX_REDUN_WORDS = (((mTParam * mMParam) / 8 + 1) / 4 + 1);
    static constexpr uint32_t BYTESTATES = 256;
    static constexpr uint32_t MAX_NUM_SYM = (2 * mTParam);

    static constexpr uint32_t ZERO_DIV_RETURN_VALUE = 1;

    static constexpr uint32_t fieldPolyTable[16] = {0x1,  // dummy value
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

    static constexpr uint32_t mMOddParam = mMParam % 2;
    static constexpr uint32_t mNParam = mFFSize - 1;
    static constexpr uint32_t mLogZVal = 2 * mNParam;

    uint32_t mNumRedundantBits, mNumRedundantBytes;
    uint32_t mNumCodeWordBytes, mNumRedundantWords;
    uint32_t mLoc[mTParam];
    uint32_t mSyndromes[MAX_NUM_SYM];
    uint32_t mTraceTestVal;
    uint32_t mQuadCompTable[mMParam];
    bool mValid;
    uint8_t mCodeWord[mNumDataBytes + 4 * MAX_REDUN_WORDS];
    uint8_t mRemainderBytes[(mTParam * mMParam) / 8 + 1];

    uint16_t aLogTable[2 * mFFSize];
    uint16_t logTable[mFFSize];
    uint32_t genPolyBitArray[mTParam * mMParam + 1];
    uint32_t genPolyDegree;
    uint32_t genPolyFdbkWords[((mTParam * mMParam) / 8 + 1) / 4 + 1];
    uint32_t encodeTable[BYTESTATES][MAX_REDUN_WORDS];
    std::bitset<mFFSize> generatedRoots;  // need the place here otherwise blow stack

    int32_t
    ffMult(int32_t a, int32_t b) const;

    int32_t
    ffInv(int32_t opa, bool& success) const;

    int32_t
    ffDiv(int32_t opa, int32_t opb, bool& success) const;

    void
    buildLogTables(void);

    bool
    checkLogTables(void) const;

    int32_t
    ffSquareRoot(int32_t opa) const;

    int32_t
    ffCubeRoot(int32_t opa, bool& success) const;

    void
    genTraceTestVal(void);

    void
    genQuadCompTable(void);

    int32_t
    ffQuadFun(int32_t c) const;

    void
    bchInit(void);

    bool
    generateCodeGenPoly(void);

    void
    convertGenPolyBitToWord(void);

    void
    generateEncodeTables(void);

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
};

}  // namespace utils
}  // namespace outpost

#include "nand_bch_runtime_impl.h"
#endif /* NAND_BCH_H_ */
