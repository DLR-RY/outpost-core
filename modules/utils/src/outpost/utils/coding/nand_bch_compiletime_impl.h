/*
 * Copyright (c) 2015, German Aerospace Center (DLR)
 * All rights reserved.
 *
 * This file is part of the RCN bootloader software.
 */
// ----------------------------------------------------------------------------
#if __cplusplus < 201402L
#error "Compile time NandBCh encoder/decoder needs at least C++14"
#else

#ifndef NAND_BCH_COMPILETIME_IMPL_H_
#define NAND_BCH_COMPILETIME_IMPL_H_

#include "nand_bch_compiletime.h"

#include <string.h>

namespace outpost
{
namespace utils
{
template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
constexpr typename NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::LogTable
        NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::aLogTable;

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
constexpr typename NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::LogTable
        NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::logTable;

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
constexpr typename NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::QuadCompTable
        NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::mQuadCompTable;

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
constexpr typename NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::Polynom
        NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::mPolynom;

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
constexpr typename NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::EncodeTable
        NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::encodeTable;

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::NandBCHCTime(void) :
    mLoc{0},
    mSyndromes{0},
    mErrLocByte{0},
    mErrLocBit{0},
    mValid(false),
    mCodeWord{0},
    mRemainderBytes{0}
{
    static_assert(checkLogTables() && mPolynom.valid,
                  "Template parameters does not create a valid encoder");

    constexpr uint8_t iteration_count = (mNandDataSize * 8) / mNumDataBits;
    mValid = (iteration_count * mNumRedundantBytes) <= mNandSpareSize;
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
constexpr typename NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::LogTable
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::buildLogTables(void)
{
    LogTable ret;
    // Constructing FF's LOG and ALOG tables
    uint32_t shiftReg = 1;
    uint32_t fdbkCon = mFFPoly - mFFSize;
    uint32_t ffSizeDivTwo = mFFSize / 2;

    for (uint32_t i = 0; i < 2 * mFFSize; i++)
    {
        if (i < mNParam)
            ret.table[shiftReg] = i;
        if (shiftReg >= ffSizeDivTwo)
            shiftReg = ((shiftReg << 1) & mNParam) ^ fdbkCon;
        else
            shiftReg <<= 1;
    }

    ret.table[0] = mLogZVal;  // log of zero

    return ret;
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
constexpr typename NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::LogTable
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::buildALogTables(void)
{
    LogTable ret;
    // Constructing FF's LOG and ALOG tables
    uint32_t shiftReg = 1;
    uint32_t fdbkCon = mFFPoly - mFFSize;
    uint32_t ffSizeDivTwo = mFFSize / 2;

    for (uint32_t i = 0; i < 2 * mFFSize; i++)
    {
        ret.table[i] = static_cast<int32_t>(shiftReg);
        if (shiftReg >= ffSizeDivTwo)
            shiftReg = ((shiftReg << 1) & mNParam) ^ fdbkCon;
        else
            shiftReg <<= 1;
    }

    ret.table[mLogZVal] = 0;
    return ret;
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
constexpr int32_t
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::ffMult(int32_t a, int32_t b)
{
    if (a == 0 || b == 0)
        return 0;

    uint16_t tmp = logTable[a] + logTable[b];

    if (tmp >= mNParam)
        tmp -= mNParam;

    return aLogTable[tmp];
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
constexpr int32_t
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::ffInv(int32_t opa, bool& success)
{
    //****************************************************************
    //  Function: ffInv
    //
    //  This is the finite field inversion (1/opa) function.  The
    //  log of the operand is fetched from a table.  The log is
    //  subtracted from field size minus 1 and the result is
    //  used to fetch an alog.
    //****************************************************************
    if (opa == 0)
    {
        success = false;  // Or into location pointed to
        return 0;
    }
    if (logTable[opa] == 0)
    {
        return aLogTable[0];  // aLogTable[0] for general case - other types of finite fields
    }
    else
    {
        return aLogTable[mNParam - logTable[opa]];
    }
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
constexpr int32_t
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::ffDiv(int32_t opa,
                                                                     int32_t opb,
                                                                     bool& success)
{
    //****************************************************************
    //  Function: ffDiv
    //
    //  This is the finite field division (opa/opb) function.  The
    //  logs of the operands are fetched.  The logs are then subtracted
    //  and the result is used to fetch an alog.
    //****************************************************************
    if (opb == 0)
    {
        success = false;  // Or into location pointed to
        return 0;
    }
    if (opa == 0)
    {
        return 0;
    }
    int32_t tmp = logTable[opa] - logTable[opb];
    if (tmp < 0)
    {
        tmp += mNParam;
    }
    return aLogTable[tmp];
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
constexpr int32_t
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::ffSquareRoot(int32_t opa)
{
    //****************************************************************
    //  Function: ffSquareRoot
    //
    //  This function returns the square root of its operand.
    //  If the finite field log of opa is even it returns -
    //  alog(log(opa)/2).  But if the finite field log of opa is
    //  odd it returns - alog((log(opa)+(finite field size -1))/2).
    //****************************************************************
    if (opa == 0)
    {
        return 0;
    }
    else
    {
        int32_t logtmp = logTable[opa];
        if (logtmp % 2 == 1)
        {
            logtmp = logtmp + mNParam;
        }
        return aLogTable[logtmp / 2];
    }
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
constexpr int32_t
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::ffCubeRoot(int32_t opa,
                                                                          bool& success)
{
    //****************************************************************
    //  Function: ffCubeRoot
    //
    //  This function returns the cube root of its operand.
    //  If the finite field log of opa is divisible by three, it
    //  returns alog(log(opa)/3).  But if the finite field log of
    //  opa is "not" divisible by three, there are no cube roots
    //  of the operand in the field and the error flag is set.
    //  The returned value in this case is not important.  This
    //  function is used only with "m" even (m of GF(2^m))
    //****************************************************************
    if (opa == 0)
    {
        return 0;
    }
    else
    {
        int32_t logtmp = logTable[opa];
        if (logtmp % 3 != 0)
        {
            success = false;  // Or error into location pointed to
        }
        return aLogTable[logtmp / 3];
    }
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
constexpr bool
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::checkLogTables(void)
{
    //****************************************************************
    //  Function: chkLogAlogTbls
    //
    //  This function tests the finite field log and alog tables to
    //  determine if they are correct.  If they are not, an error
    //  code is returned.
    //****************************************************************

    // Check log and alog tables
    for (uint32_t kx = 0; kx < mNParam; kx++)
    {
        if (kx != logTable[aLogTable[kx]])
        {
            return false;  // Flag error
        }
    }
    // Next line changed 9-5-10 for double size alog table
    if (logTable[0] != mLogZVal || aLogTable[mLogZVal] != 0)
    {
        return false;  // Flag error
    }
    return true;
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
constexpr int32_t
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::getTraceTestVal(void)
{
    int32_t traceTestVal = 0;
    int32_t shifter = 1;

    for (uint32_t jx = 0; jx < mMParam; jx++)
    {
        int32_t x = aLogTable[jx];
        int32_t sum = 0;
        for (uint32_t kx = 0; kx < mMParam; kx++)
        {
            sum ^= x;
            x = ffMult(x, x);
        }
        if (sum == 1)
        {
            traceTestVal ^= shifter;
        }
        shifter *= 2;
    }

    return traceTestVal;
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
constexpr typename NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::QuadCompTable
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::genQuadCompTable(void)
{
    //****************************************************************
    //  Function: genQuadCompTbl
    //
    //  Function to find a list of components of y to be used
    //  in finding solutions to Y^2+y+c using a table of
    //  linear components. This function is called at initialization
    //  time only.  This solution uses a small table with gblMParm
    //  entries.  A large table would be faster but would
    //  require 2^gblMParm entries.  It is possible to have an inbetween
    //  solution that uses two moderate size tables and is faster than
    //  this small table solution but not as fast as the one large table
    //  solution.  The reference for this solution is Dr. Berlekamp's 1968
    //  "Algebraic Coding Theory" book pages 243 and 244.
    //  The first loop of this function will put in the search table
    //  each "c" of y^2+y = c that has a single "1" bit or the xor of
    //  such a pattern with the value of a fixed element with
    //  trace = "1". The second loop searches for each element in the
    //  table within a set of solutions for
    //  y^2+y=c and if it finds one then it stores the associated
    //  value of y in the gblQuadCompTbl.
    //****************************************************************
    int32_t searchTable[mMParam] = {};

    int32_t firstTraceOne = 0;

    // This loop will put in the search table each "c" of y^2+y = c that has a
    // single "1" bit or the XOR of such a pattern with a fixed pattern of an
    // element with trace = "1"
    int32_t shifter = 1;
    for (uint32_t kx = 0; kx < mMParam; kx++)
    {
        searchTable[kx] = 0;
        if ((mTraceTestVal & shifter) > 0)
        {
            if (firstTraceOne > 0)
            {
                searchTable[kx] = shifter ^ firstTraceOne;
            }
            else
            {
                firstTraceOne = shifter;
            }
        }
        else
        {
            searchTable[kx] = shifter;
        }
        shifter *= 2;
    }

    QuadCompTable ret;

    // This loop will search for each "c" of y^2+y = c that has a single bit
    // or is the xor of such a pattern with a fixed pattern of an
    // element with trace = "1"
    for (uint32_t y = 0; y < mFFSize; y += 2)
    {  // Incr of 2 may not work with another basis

        int32_t c = ffMult(y, y) ^ y;
        for (uint32_t kx = 0; kx < mMParam; kx++)
        {
            if (c == searchTable[kx])
            {
                ret.table[kx] = y;
            }
        }
    }

    return ret;
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
constexpr int32_t
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::ffQuadFun(int32_t c)
{
    //****************************************************************
    // Function: ffQuadFun
    //
    // This function is used during decode operations of
    // error correction.  When a solution is needed for the equation
    // y^2+y+c=0 the function first ANDs the trace
    // test value with "c" and then tests parity
    // of the resulting bits.  If parity is odd (trace = 1)
    // an error is posted and the function exits.  If parity is
    // even then the function continues on and XORs a set of values to
    // get y1, one solution of y^2+y=c.  The second solution is the
    // XOR of the first solution with a single one bit in the low
    // order position.
    //
    // This function can be made faster with a single large table.  This
    // table would be the same size as the log and alog tables.  So
    // for large "m" (m of GF(2^m)) the table will be large.  Using
    // lots of memory may not be good for a firmware application but
    // might be ok for a PC application.
    //****************************************************************
    // "c" is "c" of y^2+y=c.  Given "c" we want one solution (y1) of y^2+y=c
    int32_t parityWord = mTraceTestVal & c;  // Get bits that parity is to be checked on
    int32_t parityBit = 0;
    int32_t y1 = 0;
    int32_t shifter = 1;
    for (uint32_t kx = 0; kx < mMParam; kx++)
    {
        if ((c & shifter) > 0)
            y1 ^= mQuadCompTable[kx];  // XOR values from the quadratic component table

        if ((parityWord & shifter) > 0)
            parityBit ^= 1;  // Accumulate parity to get trace of "c"

        shifter *= 2;
    }

    if (parityBit > 0)
    {
        return 0;  // If trace of "c" is "1"
    }
    else
    {
        return y1 ^ 1;  // "1" Not for all basis.  y1 is one solution of y^2+y=c.
    }
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
constexpr typename NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::Polynom
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::generateCodeGenPoly(void)
{
    //****************************************************************
    //  Function: genCodeGenPoly
    //
    //  Function to compute a code generator polynomial for a binary
    //  BCH code.  The action of this function is equivalent to finding
    //  the polynomial that is the least common multiple of a set of
    //  minimum polynomials.
    //
    //  This function was motivated by a similar function in the
    //  Robert Morelos-Zaragoza bch decoder on the "ECC Page" web site.
    //
    //  Generally, finding the code generator polynomial requires two
    //  steps  1) compute minimum polynomials and 2) find the LCM of
    //  the minimum polynomials to determine the code generator polynomial.
    //  But it is also possible to combine these steps.  Finding a
    //  minimum polynomial is accomplished by finding the product of a
    //  set of roots and finding the code generator polynomial is
    //  accomplished by finding the product of a set of minimum
    //  polynomials.  So the two steps can be combined to find the code
    //  generator polynomial in one step by finding the product of a
    //  larger set of roots.  This program uses this combined approach.
    //
    //  It might be possible though to pull a little time out of this function
    //  by using the more general two step approach.  By careful coding it may
    //  be possible to take advantage of the fact that once the minimum
    //  polynomials have been computed, computing the code generator polynomial
    //  from them involves only GF(2) finite field operations.
    //
    //  This function is used only during development, so it would not be part
    //  of an implementation in a product employing one fixed code.
    //
    //  NOTE: The code generator polynomial is stored in a array,
    //  one bit per int32_t and Low order in address 0.
    //
    //  Note to Neal: On next update check value of root before using it
    //  as an index to store "1" in the flg array.  This will reduce
    //  significantly the size of the flg array and will reduce the time
    //  to initialize this array.
    //****************************************************************
    Polynom ret;

    uint32_t genPolyBitArray[mTParam * mMParam + 1] = {};

    int32_t flag[mFFSize] = {};
    int32_t tmp[mTParam * mMParam + 1] = {};

    ret.genPolyDegree = 0;   // The degree of the code generator poly is obj->mInitialized to "0"
    genPolyBitArray[0] = 1;  // Now the initial code generator poly is "1" (degree "0")

    for (uint32_t rootBase = 1; rootBase <= 2 * mTParam - 1; rootBase += 2)
    {  // alpha 1,3,5,7 etc.
        if (flag[rootBase] == 0)
        {  // If this root not already processed
            uint32_t root = rootBase;
            for (;;)
            {  // Infinite loop - Exit is by "break"
                // In loop - root will take values like 1,2,4,8... 3,6,12,24...etc
                if (ret.genPolyDegree + 1 > mMParam * mTParam)
                {
                    ret.valid = false;
                    return ret;
                }

                // Start multiply this factor times current gblCgpBitArray
                // Intermediate poly products will have some values greater than one
                // The coefficients of the final product will have values 0 or 1 only.
                // Multiply code gen poly by (X - alpha^root)
                for (int32_t kx = ret.genPolyDegree + 1; kx >= 1; kx--)
                {
                    // Shift poly 1 place (low to high)
                    tmp[kx] = genPolyBitArray[kx - 1];
                }
                tmp[0] = 0;
                ret.genPolyDegree++;
                for (int32_t kx = ret.genPolyDegree; kx >= 0; kx--)
                {
                    if (genPolyBitArray[kx] > 0)
                    {
                        int32_t wk = ffMult(genPolyBitArray[kx], aLogTable[root]);
                        genPolyBitArray[kx] = tmp[kx] ^ wk;
                    }
                    else
                    {
                        genPolyBitArray[kx] = tmp[kx];  // Move the shifted version of cgp
                    }
                }
                // End multiply
                flag[root] = 1;
                root *= 2;  // root is in finite field log form
                if (root >= mNParam)
                {
                    root -= mNParam;  // These 3 lines do a fast mod op
                }
                if (root == rootBase)
                {
                    break;
                }
            }
        }
    }
    ret.valid = true;
    convertGenPolyBitToWord(ret, genPolyBitArray);
    return ret;
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
constexpr void
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::convertGenPolyBitToWord(
        Polynom& poly, const uint32_t* genPolyBitArray)
{
    //****************************************************************
    //  Function: cvtCgpBitToCgpWord
    //
    //  Function to convert, the code generator polynomial (cgp) stored one
    //  bit per int32_t and organized low order to address 0, into a set of
    //  words that contain the shift register feedback pattern for the code
    //  generator polynomial.  The feedback words are stored high order to
    //  address 0.  SO THE ORGANIZATION CHANGES BETWEEN THE INPUT AND OUTPUT
    //  OF THIS FUNCTION.
    //  GenPolyBits [0 ... 103] : GenPolyWord0 = GenPolyBit [103 ... 70]
    //  GenPolyBits [0 ... 103] : GenPolyWord1 = GenPolyBit [71  ... 40]
    //  GenPolyBits [0 ... 103] : GenPolyWord2 = GenPolyBit [39  ... 8 ]
    //  GenPolyBits [0 ... 103] : GenPolyWord3 = GenPolyBit [7   ... 0 ]
    //****************************************************************

    uint32_t wordAddr = 0;
    uint32_t bitMask = 0x80000000;

    // Place the code generator poly in the ints
    for (int32_t kkk = poly.genPolyDegree - 1; kkk >= 0; kkk--)
    {
        if (genPolyBitArray[kkk] > 0)
        {
            poly.genPolyFdbkWords[wordAddr] ^= bitMask;
        }
        bitMask >>= 1;  // bitMask must be unsigned
        if (bitMask == 0)
        {
            bitMask = 0x80000000;
            wordAddr++;
        }
    }
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
constexpr typename NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::EncodeTable
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::generateEncodeTables(void)
{
    //****************************************************************
    //  Function: genEncodeTbls
    //
    //  Function to generate encode tables.  The encode tables allow
    //  8 bits of data to be processed at a time.  A Left shifting shift
    //  reg is implemented in multiple 32-bit words.  A multiple word
    //  shift register value is stored in the table for each possible
    //  value of a byte.  A byte value is placed in the left most
    //  (high order position of lowest address word).  Then the shift
    //  register is shifted 8 times with feedback.  Then the multiple
    //  word shift register value is stored in the table at the addr
    //  specified by the byte.
    //
    //  The feedback words are highest order in lowest address and the
    //  resulting encode table is organized the same way.
    //****************************************************************
    EncodeTable ret;

    uint32_t SR[mNumRedundantWords] = {};

    // Gen Encode Table
    for (uint32_t i = 0; i < BYTESTATES; i++)
    {  // Encoding is 8 bits parallel
        // Clear shift register
        for (uint32_t pos = 0; pos < mNumRedundantWords; pos++)
        {
            SR[pos] = 0;
        }

        SR[0] ^= (i << 24);
        for (uint32_t j = 0; j <= 7; j++)
        {  // 7 is the # of bits in a byte -1
            uint32_t fdbk = 0;
            for (int32_t nnn = mNumRedundantWords - 1; nnn >= 0; nnn--)
            {
                uint32_t fdbkSav = fdbk;
                if (SR[nnn] & 0x80000000)
                {
                    fdbk = 1;
                }
                else
                {
                    fdbk = 0;
                }
                SR[nnn] <<= 1;
                SR[nnn] ^= fdbkSav;
            }
            if (fdbk == 1)
            {
                for (uint32_t k = 0; k < mNumRedundantWords; k++)
                {
                    SR[k] ^= genPolyFdbkWords[k];
                }
            }
        }
        for (uint32_t k = 0; k < mNumRedundantWords; k++)
        {
            ret[i][k] = SR[k];  // Move SR to encode table
        }
    }

    return ret;
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
void
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::bchEncode(void)
{
    //****************************************************************
    //  Function: bchEncode
    //
    //  This is the encoder.  It performs its function byte parallel using
    //  an encode table that is built during initialization.  It processes
    //  one byte at a time regardless of the size of the finite field.
    //
    //  The parallel approach shifts a software shift register implementing the
    //  code generator polynomial once per obj->mCodeWord byte even though the binary
    //  BCH code generator polynomial is over GF(2).  The shift occurs once per
    //  byte regardless of the size of the finite field.  The value of the
    //  obj->mCodeWord byte being processed addresses a large table to get the next
    //  value of the software shift register.  See page 243 (k-bit serial techniques)
    //  of "Practical Error Correction Design for Engineers" (revised second
    //  edition 1991) by Neal Glover and Trent Dudley.  Page 347 of this book
    //  describes tables for an early implementation of k-bit serial for
    //  a binary code (a computer generated code).  The k-bit serial
    //  method described there would be the same as for a binary
    //  BCH code.  I learned early on that it was possible to implement
    //  a parallel shift register for a binary code.  I may have learned this
    //  from a paper by Hsiao and Sih titled "Serial-to-Parrallel Transformation
    //  of Linear-Feedback Shift-Register Circuits" which appeared in
    //  IEEE. Trans. on Elec. Comp., 738-740 (Dec. 1964).
    //****************************************************************
    uint32_t SR[mNumRedundantWords];

    memset(SR, 0, mNumRedundantWords * 4);

    for (uint16_t writeCWAddr = 0; writeCWAddr < mNumDataBytes; writeCWAddr++)
    {  // index to write data buffer
        uint32_t fdbk = 0;
        for (int32_t i = mNumRedundantWords - 1; i >= 0; i--)
        {
            // IT IS POSSIBLE TO UNROLL THIS LOOP USING A SWITCH STATEMENT.
            // WILL HAVE TO PROCESS FROM HIGH ADDRESS BACK TOWARDS ZERO
            uint32_t fdbkSav = fdbk;
            // 32 bits per int32_t - but process 8 bits at a time (8 bits unrelated to "m")
            fdbk = (SR[i] >> 24) & 0x000000ff;
            SR[i] = (SR[i] << 8) ^ fdbkSav;  // 8 # bits in parallel - unrelated to "m"
        }
        fdbk ^= mCodeWord[writeCWAddr];
        // IT IS POSSIBLE TO UNROLL THIS LOOP USING A SWITCH STATEMENT
        for (uint32_t kx = 0; kx < mNumRedundantWords; kx++)
        {
            SR[kx] ^= encodeTable[fdbk][kx];
        }
    }

    // +5 So that we can temporarily keep remainder bytes in whole words
    int32_t redunByteArray[(mTParam * mMParam) / 8 + 5];
    // Copy redundancy bytes from shift register (SR) word array
    for (uint32_t kx = 0; kx < mNumRedundantWords; kx++)
    {
        uint32_t tmp = SR[kx];  // Fetch a 32 bit word
        redunByteArray[4 * kx] = tmp >> 24;
        redunByteArray[4 * kx + 1] = (tmp >> 16) & 0x000000ff;
        redunByteArray[4 * kx + 2] = (tmp >> 8) & 0x000000ff;
        redunByteArray[4 * kx + 3] = tmp & 0x000000ff;
    }
    // Copy redundancy to obj->mCodeWord.  Don't copy pad bytes of SR word array
    for (uint32_t kx = 0; kx < mNumRedundantBytes; kx++)
    {
        mCodeWord[mNumDataBytes + kx] = redunByteArray[kx];
    }
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
int32_t
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::computeRemainder(void)
{
    //****************************************************************
    //  Function: computeRemainder
    //
    //  This function computes a remainder.  This remainder is the remainder
    //  from dividing the received code word (obj->mCodeWord) by the full code
    //  generator polynomial (not by factors of it). This is the key to speeding
    //  up syndrome computation.
    //  This remainder is computed using a parallel approach similar to that used
    //  in encoding.  Once we have a full remainder we can compute syndrome
    //  (obj->mSyndromes) from it.
    //  This is faster than computing syndromes directly from the code word
    //  because the remainder is much shorter than the code word.
    //
    //  This also gives us a fast way to determine if the remainder would all be
    //  zero. If the remainder is all zeros then there is no need to compute
    //  syndrome from it as they would all be zeros also.  If the remainder is
    //  zero then the code word is assumed to be error free and no further
    // decoding steps are necessary.
    //
    //  The parallel approach shifts a software shift register implementing the
    //  code generator polynomial once per code word byte even though the binary
    //  BCH code generator polynomial is over GF(2). The shift occurs once per
    //  byte regardless of the size of the finite field.  The value of the
    //  code word byte being processed addresses a large table to get the next
    //  value of the software shift register.  See page 243 (k-bit serial techniques)
    //  of "Practical Error Correction Design for Engineers" (revised second
    //  edition 1991) by Neal Glover and Trent Dudley.  Page 347 of this book
    //  describes tables for an early implementation of k-bit serial for
    //  a binary code (a computer generated code).  The k-bit serial
    //  method described there would be the same as for a binary
    //  BCH code. I learned early on that it was possible to implement
    //  a parallel shift register for a binary code.  I may have learned this
    //  from a paper by Hsiao and Sih titled "Serial-to-Parrallel Transformation
    //  of Linear-Feedback Shift-Register Circuits" which appeared in
    //  IEEE. Trans. on Elec. Comp., 738-740 (Dec. 1964).
    //****************************************************************
    uint32_t SR[mNumRedundantWords];

    // Clear shift register
    memset(SR, 0, mNumRedundantWords * 4);

    // SHIFTS WITH FEEDBACK
    for (uint32_t readCWAddr = 0; readCWAddr < mNumDataBytes; readCWAddr++)
    {  // index to write data buffer
        uint32_t fdbk = 0;
        for (int32_t nnn = mNumRedundantWords - 1; nnn >= 0; nnn--)
        {
            // IT IS POSSIBLE TO UNROLL THIS LOOP USING A SWITCH STATEMENT.
            // WILL HAVE TO PROCESS FROM HIGH ADDRESS BACK TOWARDS ZERO
            uint32_t fdbkSav = fdbk;
            fdbk = (SR[nnn] >> 24);              // 32 - # bits in word (unrelated to "m")
            SR[nnn] = (SR[nnn] << 8) ^ fdbkSav;  // 8 # bits in parallel - unrelated to "m"
        }
        fdbk ^= mCodeWord[readCWAddr];
        for (uint32_t k = 0; k < mNumRedundantWords; k++)
        {
            // IT IS POSSIBLE TO UNROLL THIS LOOP
            SR[k] ^= encodeTable[fdbk][k];
        }
    }
    // SHIFTS WITHOUT FEEDBACK
    // Line below - This flag will be set later if the remainder is non zero.
    // Non-zero means either corr or uncorr err.  We will know which after decoding.
    int32_t remainderDetdErr = 0;

    // index to code word buffer
    for (uint32_t readCWAddr = mNumDataBytes; readCWAddr < mNumDataBytes + mNumRedundantBytes;
         readCWAddr++)
    {
        uint32_t fdbk = 0;
        for (int32_t nnn = mNumRedundantWords - 1; nnn >= 0; nnn--)
        {
            uint32_t fdbkSav = fdbk;
            fdbk = (SR[nnn] >> 24);              // 32 - # bits in parallel (unrelated to "m")
            SR[nnn] = (SR[nnn] << 8) ^ fdbkSav;  // 8 # bits in parallel - unrelated to "m"
        }
        fdbk ^= mCodeWord[readCWAddr];
        mRemainderBytes[readCWAddr - mNumDataBytes] = static_cast<int32_t>(fdbk);
        if (fdbk != 0)
        {
            remainderDetdErr = 1;
        }
    }
    return remainderDetdErr;
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
void
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::computeSyndromes(void)
{
    //****************************************************************
    //  Function: computeSyndromes
    //
    //  9-10-10 Made few changes for speed
    //
    //  Function to compute syndromes.  Syndromes are computed from a
    //  remainder not from the code word.  See the comments
    //  for the ComputeRemainder function.
    //
    //  Note that even syndromes are computed from odd syndromes.
    //  Reference to this is from an early book by Peterson, Berlekamp, or
    //  Shu Lin.  I just found an early reference to this on
    //  page 192 of "Algebraic Coding Theory" by Dr. Elwyn
    //  Berlekamp.
    //
    //  An early reference for computing syndromes from
    //  a remainder is page 160 of the Glover-Dudley 1991 book
    //  "Practical Error Correction Design for Engineers"
    //  REVISED SECOND EDITION.
    //****************************************************************

    // In a real implementation of one fixed code, numSyndromes would be a constant
    uint32_t numSyndromes = 2 * mTParam;

    // Clear syndromes array
    memset(mSyndromes, 0, numSyndromes * sizeof(uint32_t));

    for (uint32_t i = 0; i < mNumRedundantBytes; i++)
    {
        int32_t mask = 0x01;
        for (uint32_t j = 0; j < 8; j++)
        {
            int32_t data = mRemainderBytes[i] & mask;
            if (data > 0)
            {
                // 9-10-10 Changed for speed
                uint32_t accumVal = (((mNumRedundantBytes - 1) - i) * 8 + j);  // Initialize
                uint32_t bumpVal = 2 * accumVal;                               // Initialize
                // Note increment by 2
                for (uint32_t k = 0; k < numSyndromes; k += 2)
                {
                    //  "+1" is for syndrome offset
                    //  CAN UNROLL THIS LOOP FOR A LITTLE MORE SPEED
                    //  A SWITCH STATEMENT WILL BE REQUIRED.
                    //  CAN GET RID OF MOD (FOR SPEED) BY CARRYING A SUM THAT STARTS
                    //  OUT AS THE MAXIMUM VALUE OF ((kkk+1)*preComputeVal) AND
                    //  SUBTRACTING PRECOMPUTEVAL EACH TIME AND IF VALUE GOES
                    //  NEGATIVE ADD IN NPARM.  THE MAX VALUE ABOVE WILL VARY
                    //  BASED ON NUMBER OF REDUN BYTES FOR SELECTED PARMS
                    //  9-10-10 Changed next few lines for speed
                    mSyndromes[k] ^= aLogTable[accumVal];
                    accumVal += bumpVal;  // Add 2*preComputeVal
                    if (accumVal >= mNParam)
                    {
                        accumVal -= mNParam;
                        if (accumVal >= mNParam)
                        {
                            accumVal -= mNParam;
                        }
                    }
                }
            }
            mask *= 2;
        }
    }
    // Compute even syndromes from the odd syndromes
    for (uint32_t k = 0; k < numSyndromes; k += 2)
    {
        uint32_t x = mSyndromes[k];
        uint32_t evenSNum = (2 * (k + 1));
        while (evenSNum <= 2 * mTParam)
        {
            // Square "x"
            if (x > 0)
            {
                x = aLogTable[2 * logTable[x]];
            }
            mSyndromes[evenSNum - 1] = x;
            evenSNum *= 2;
        }
    }
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
uint32_t
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::berMas(int32_t sigmaN[],
                                                                      bool& success)
{
    //***************************************************************
    //  Function: BerMas
    //
    //  10-3-08 One line was changed and one line was added to change the
    //  the code over from supporting a Reed-Solomon code to supporting a
    //  binary BCH code.
    //
    //  This function computes the coefficient's of the ELP using the Berlekamp-
    //  Massey algorithm. The reference for this algorithm is
    //  "Shift-Register Synthesis and BCH Decoding", IEEE Transactions
    //  on Information Theory, IT-15, 122-127, 1969.  This paper can also be
    //  found in Blake's 1973 book, "Algebraic Coding Theory"
    //  (a collection of papers).  When I wrote the 1999 version of this
    //  function, I evaluated several variations of the Berlekamp-Massey
    //  algorithm. The variations vary mostly in the initialization of "nn"
    //  (0 or 1) and in the algebra used for the decisions.
    //
    //  I wrote this code originally in 1999 for a Reed-Solomon code.
    //  I added the error evaluator polynomial at that time.  My reference
    //  for adding the error evaluator polynomial was the thesis
    //  by Doug Whiting.  I removed the error evaluator polynomial 10-3-08
    //  for a binary BCH, since we do not compute error values for a binary
    //  BCH code.  Variable names used here are related to the variable names
    //  used in the original Massey article as shown below.
    //
    //   This Code   Massey Paper
    //   ---------   ------------
    //   sigmaTmp    T(D)  D of T(D) is from signal processing and is equiv to using T(x)
    //   SigmaN      C(D)
    //   sigmaK      B(D)
    //   n           N
    //   nminusk     x
    //   dk          b
    //   dn          d
    //   Ln          L
    //
    //  For implementations where the time spent in this function is a
    //  dominating factor in performance, a technique due to C.L. Chen can
    //  be used for a speed up. If you are interested see "High-Speed
    //  Decoding of BCH Codes" by C.L. Chen - IEEE Info Theory VOL. IT-27,
    //  NO. 2, March 1981.  Before implementing this technique you need
    //  to understand its impact on your miscorrection rate.
    //***************************************************************
    int32_t sigmaK[mTParam + 1];
    int32_t sigmaTmp[mTParam + 1];

    sigmaN[0] = 1;
    sigmaK[0] = 1;
    for (uint32_t n = 1; n <= mTParam; n++)
    {
        sigmaN[n] = 0;
        sigmaK[n] = 0;
    }
    int32_t nminusk = 1;
    int32_t dk = 1;
    uint32_t lk = 0;
    uint32_t Ln = 0;
    // Next cmd line - 2*tParm is number of obj->mSyndromes
    // Next cmd line - *****incr was changed from 1 (RS) to 2 (bin BCH)*****
    for (uint32_t n = 0; n < 2 * mTParam; n += 2)
    {
        int32_t dn = 0;
        for (uint32_t j = 0; j <= Ln; j++)
        {
            dn ^= ffMult(sigmaN[j], mSyndromes[n - j]);
        }
        if (dn == 0)
        {
            nminusk++;
        }
        else
        {
            if (2 * Ln > n)
            {
                if (n - Ln > (mTParam - 1))
                {
                    success = false;
                    break;
                }
                // Next 2 "if" blks chgd to not use ffMult and ffDiv funs 9-9-2010
                if (dk == 0)
                {
                    return (ZERO_DIV_RETURN_VALUE);  // Divide by zero error
                }
                if (dn > 0)
                {
                    int32_t logTmpQ = logTable[dn] - logTable[dk];
                    if (logTmpQ < 0)
                    {
                        logTmpQ += mNParam;
                    }
                    for (uint32_t j = 0; j <= lk; j++)
                    {
                        if (sigmaK[j] > 0)
                        {
                            sigmaN[nminusk + j] ^= aLogTable[logTable[sigmaK[j]] + logTmpQ];
                        }
                    }
                }
                nminusk++;
            }
            else
            {
                if (n - Ln > (mTParam - 1))
                {
                    success = false;
                    break;
                }
                for (uint32_t j = 0; j <= Ln; j++)
                {
                    sigmaTmp[j] = sigmaN[j];
                }
                // Next 2 "if" blks chgd to not use ffMult and ffDiv funs 9-9-2010
                if (dk == 0)
                {
                    return (ZERO_DIV_RETURN_VALUE);  // Divide by zero error
                }
                if (dn > 0)
                {
                    int32_t logTmpQ = logTable[dn] - logTable[dk];
                    if (logTmpQ < 0)
                    {
                        logTmpQ += mNParam;
                    }
                    for (uint32_t j = 0; j <= lk; j++)
                    {
                        if (sigmaK[j] > 0)
                        {
                            sigmaN[nminusk + j] ^= aLogTable[logTable[sigmaK[j]] + logTmpQ];
                        }
                    }
                }
                for (uint32_t j = 0; j <= Ln; j++)
                {
                    sigmaK[j] = sigmaTmp[j];
                }
                lk = Ln;
                Ln = n + 1 - Ln;
                dk = dn;
                nminusk = 1;
            }
        }
        nminusk++;  // *****This line added for binary BCH*****
    }               // end for
    if (sigmaN[Ln] == 0)
    {
        success = false;
    }
    return (Ln);
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
bool
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::chienSearch(int32_t sigmaN[],
                                                                           const int32_t LnOrig)
{
    //****************************************************************
    //  Function:   ChienSearch
    //
    //  This is an enhanced version of the Chien search algorithm.  As
    //  coded here the function finds roots until the degree (Ln) of
    //  the Error Locator Polynomial (ELP) has been reduced to four,
    //  if "m" is even, or to two, if "m" is odd.  The quartic function
    //  finds the last four roots if "m" is even or the quadratic function
    //  finds the last two roots if "m" is odd.  This function is not
    //  called if the degree of the original ELP is 4 or less("m" even) or 2
    //  or less ("m" odd).  To optimize speed two different techniques are
    //  used to advance the ELP for its next evaluation.
    //
    //  For this code a fast technique is used when there are no zero
    //  coefficients in the ELP.  A slower technique is used when there are
    //  zero coefficients in the ELP. The probability that there are zero
    //  coefficients in the ELP is relatively low.  Also for speed the ELP
    //  is divided down each time a root is found.  Also for speed the inner
    //  most loop of the faster technique is unrolled.  Again for speed, mod
    //  operations are replaced with equivalent but faster operations.
    //  My original knowledge of unrolling loops came from the Earl
    //  Cohen Ph.D. thesis (Berkeley).  See also the Glover-Dudley patent
    //  4,839,896.
    //
    //  The fastest Chien search technique that I am aware of is to
    //  keep the coefficient values in alog form and to use a table
    //  in memory to implement each constant multiplier of the Chien
    //  search.  With this technique combined with loop unrolling
    //  I think you would see a speed up by about a factor of two
    //  for root finding.  The memory space requirement will be very
    //  large for large finite fields.  The large memory requirement
    //  may not be a problem for decoders that run on a PC.
    //
    //  There are software root finding techniques that beat the
    //  speed of even very well written Chien search software.
    //  Some of these techniques are for particular cases such
    //  as for quintic and sextic polynomials.  But others are for
    //  the general case.  Contact the author for more information.
    //
    //  The original reference for the most basic Chien search is
    //  "Cyclic Decoding Procedures for Bose-Chaudhuri-
    //  Hocquenghem Codes", IEEE Transactions on Information Theory,
    //  vol. IT-10, pp 357-363, Oct. 1964.
    //
    //****************************************************************
    uint32_t Ln = LnOrig;
    // Check for a zero coeff before converting to log domain and if find one, set flag
    int32_t coeffContainsAZero = 0;
    for (uint32_t i = 1; i <= Ln; i++)
    {
        if (sigmaN[i] == 0)
        {
            coeffContainsAZero = 1;
            break;
        }
    }
    // Convert error locator poly to log domain for Chien Search
    for (uint32_t n = 1; n <= Ln; n++)
    {
        sigmaN[n] = logTable[sigmaN[n]];
    }
    for (uint32_t n = 0; n < mNumCodeWordBytes * 8; n++)
    {
        int32_t accum = 0;
        // Do simple Chien Search if zero coeffs or if Ln>8.  YOU CANNOT
        // CHANGE THE # 8 IN "Ln>8" WITHOUT ADDING MORE ENTRIES FOR THE
        // UNROLLED LOOP
        if (coeffContainsAZero == 1 || Ln > 8)
        {
            for (uint32_t jj = 1; jj <= Ln; jj++)
            {  // One step of Simple Chien Search in this loop
                if (sigmaN[jj] != static_cast<int32_t>(mLogZVal))
                {  // Test for log of zero
                    accum ^= aLogTable[sigmaN[jj]];
                    sigmaN[jj] -= jj;
                    if (sigmaN[jj] < 0)
                    {  // Compare & subtract is faster than mod
                        sigmaN[jj] += mNParam;
                    }
                }
            }
        }
        else
        {
            // Unrolled loop
            switch (Ln)
            {
                case 8:
                    accum ^= aLogTable[sigmaN[8]];  // accum is XOR sum of all alogs
                    sigmaN[8] -= 8;
                    if (sigmaN[8] < 0)
                    {
                        sigmaN[8] += mNParam;
                    }
                    // lint -fallthrough
                case 7:
                    accum ^= aLogTable[sigmaN[7]];  // accum is XOR sum of all alogs
                    sigmaN[7] -= 7;
                    if (sigmaN[7] < 0)
                    {
                        sigmaN[7] += mNParam;
                    }
                    // lint -fallthrough
                case 6:
                    accum ^= aLogTable[sigmaN[6]];  // accum is XOR sum of all alogs
                    sigmaN[6] -= 6;
                    if (sigmaN[6] < 0)
                    {
                        sigmaN[6] += mNParam;
                    }
                    // lint -fallthrough
                case 5:
                    accum ^= aLogTable[sigmaN[5]];  // accum is XOR sum of all alogs
                    sigmaN[5] -= 5;
                    if (sigmaN[5] < 0)
                    {
                        sigmaN[5] += mNParam;
                    }
                    // lint -fallthrough
                case 4:
                    accum ^= aLogTable[sigmaN[4]];  // accum is XOR sum of all alogs
                    sigmaN[4] -= 4;
                    if (sigmaN[4] < 0)
                    {
                        sigmaN[4] += mNParam;
                    }
                    // lint -fallthrough
                case 3:
                    accum ^= aLogTable[sigmaN[3]];  // accum is XOR sum of all alogs
                    sigmaN[3] -= 3;
                    if (sigmaN[3] < 0)
                    {
                        sigmaN[3] += mNParam;
                    }
                    // lint -fallthrough
                case 2:
                    accum ^= aLogTable[sigmaN[2]];  // accum is XOR sum of all alogs
                    sigmaN[2] -= 2;
                    if (sigmaN[2] < 0)
                    {
                        sigmaN[2] += mNParam;
                    }
                    // lint -fallthrough
                case 1:
                    accum ^= aLogTable[sigmaN[1]];  // accum is XOR sum of all alogs
                    sigmaN[1] -= 1;
                    if (sigmaN[1] < 0)
                    {
                        sigmaN[1] += mNParam;
                    }
                    // lint -fallthrough
                default: break;
            }
        }
        if (accum == 1)
        {
            mLoc[Ln - 1] = aLogTable[n];
            // Convert back to alog domain so we can divide down
            for (uint32_t i = 1; i <= Ln; i++)
            {
                sigmaN[i] = aLogTable[sigmaN[i]];
            }
            // Divide down the ELP to eliminate the root just found
            int32_t reg = 0;
            for (int32_t kx = Ln; kx >= 0; kx--)
            {
                int32_t tmp = ffMult(reg, aLogTable[1]);  // The number "1"
                reg = sigmaN[kx] ^ tmp;
                sigmaN[kx] = tmp;
            }
            Ln--;  // Ln must be decremented right here - do not move
            // Check if any coefficients are zero and set flag appropriately
            coeffContainsAZero = 0;
            for (uint32_t i = 1; i <= Ln; i++)
            {
                if (sigmaN[i] == 0)
                {
                    coeffContainsAZero = 1;
                    break;
                }
            }
            // If degree reduced, special cases will take it from here
            if ((Ln == 4 && mMOddParam == 0) || (Ln == 2 && mMOddParam == 1))  // 4 and 2
            {
                // We are still in alog domain so,
                // position the ELP back to its starting point for special cases
                for (uint32_t i = 1; i <= Ln; i++)
                {
                    sigmaN[i] = ffMult(sigmaN[i], aLogTable[((n + 1) * i) % mNParam]);
                }
                break;
            }
            // Convert back to log domain so we can continue root search
            for (uint32_t i = 1; i <= Ln; i++)
            {
                sigmaN[i] = logTable[sigmaN[i]];
            }
        }
    }
    // If degree of ELP has not been reduced properly
    if ((Ln != 4 && mMOddParam == 0) || (Ln != 2 && mMOddParam == 1))
    {  // 4 and 2
        return false;
    }
    else
    {
        return true;
    }
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
bool
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::quadraticElp(const int32_t sigmaN[])
{
    //****************************************************************
    //  Function:  quadraticElp
    //
    //  Function to find the roots of a degree two error locator
    //  polynomial (ELP).  References for this algorithm is the 1978
    //  Flagg patent 4,099,160 and the paper "High Speed Interleaved
    //  Reed-Solomon Error Detection and Correction System" by
    //  Shirish Deodhar and E.J. Weldon, Journal of Photo-Optical
    //  Instrumentation Engineers (SPIE), 1983.  See also the Deodhar
    //  patent 4,567,594 and the Glover-Dudley patents 4,839,896 and
    //  5,280,488.
    //****************************************************************
    bool success = true;
    if (sigmaN[1] == 0)
    {
        success = false;
    }
    int32_t c = ffDiv(sigmaN[2], ffMult(sigmaN[1], sigmaN[1]), success);

    int32_t y1 = ffQuadFun(c);

    if (y1 == 0)
    {
        success = false;
    }

    // In next line, if ever use basis other than poly then "1" may have
    // to change to alog[0]
    int32_t y2 = y1 ^ 1;

    mLoc[0] = ffMult(sigmaN[1], y1);
    mLoc[1] = ffMult(sigmaN[1], y2);

    return success;
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
bool
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::cubicElp(const int32_t sigmaN[])
{
    //****************************************************************
    //  Function:   cubicElp
    //
    //  Function to find the roots of a degree three error locator
    //  polynomial (ELP).  This code follows the algorithm of page 355
    //  of the 1991 Glover-Dudley book "Practical Error Correction Design
    //  for Engineers" REVISED SECOND EDITION.  My original reference for
    //  that book was the 1978 Flagg patent 4,099,160.  See also
    //  the paper "High Speed Interleaved Reed-Solomon Error
    //  Detection and Correction System" by Shirish Deodhar
    //  and E.J. Weldon, Journal of Photo-Optical
    //  Instrumentation Engineers (SPIE), 1983.  See also the Deodhar
    //  patent 4,567,594 and the Glover-Dudley patents 4,839,896 and
    //  5,280,488.  See also the paper by Chien, Cunningham, and
    //  Oldham titled "Hybrid Methods for Finding Roots of a Polynomial
    //  With Application to BCH Decoding" published in Transactions on
    //  Information Theory", March 1969 pages 329-335.
    //
    //  Note to Neal. ###### Next level - list here the conditions that
    //  result in an uncorrectable error and where each is detected.
    //***************************************************************

    bool success = true;
    // n for numerator, d for denominator
    int32_t n = sigmaN[2] ^ ffMult(sigmaN[1], sigmaN[1]);
    int32_t d = sigmaN[3] ^ ffMult(sigmaN[1], sigmaN[2]);
    // Note to Neal.  The error check on the next line is
    // redundant. This error would also get caught in "ffDiv" function.
    if (d == 0)
    {
        success = false;  // Divide by "0" error
    }
    int32_t n3 = ffMult(n, ffMult(n, n));  // Numerator cubed
    int32_t d2 = ffMult(d, d);             // Denominator squared
    int32_t c = ffDiv(n3, d2, success);    // Finite field divide
    // Note to Neal.  ######## I think I put in the next decision
    // during debug in 1999.  The code could be extensively tested
    // without this decision to see if it can be left out.
    int32_t u1;
    if (n == 0)
    {
        u1 = d;
    }
    else
    {
        // The quad function is equiv to fetching from large table
        int32_t v1 = ffQuadFun(c);
        if (v1 == 0)
        {
            success = false;
        }
        u1 = ffMult(v1, d);
    }
    // Roots of transformed cubic
    int32_t t1 = ffCubeRoot(u1, success);
    int32_t t2 = ffMult(t1, aLogTable[mNParam / 3]);  // nParm/3 is 85 for gf(2^8)
    int32_t t3 = t1 ^ t2;  // Equivalent to t2= line with nParm replaced by 2*nParm
    // Roots of original cubic
    mLoc[0] = sigmaN[1] ^ t1 ^ ffDiv(n, t1, success);
    mLoc[1] = sigmaN[1] ^ t2 ^ ffDiv(n, t2, success);
    mLoc[2] = sigmaN[1] ^ t3 ^ ffDiv(n, t3, success);

    return success;
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
bool
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::quarticElp(int32_t sigmaN[])
{
    //****************************************************************
    //  Function:   quarticElp
    //
    //  Function to find the roots of a degree four error locator
    //  polynomial (ELP).  My code follows the quartic algorithm
    //  of the paper "High Speed Interleaved Reed-Solomon
    //  Error Detection and Correction System" by
    //  Shirish Deodhar and E.J. Weldon, Journal of Photo-Optical
    //  Instrumentation Engineers (SPIE), 1983.  See also the 1978
    //  Flagg patent 4,099,160, the Deodhar patent 4,567,594,
    //  and the Glover-Dudley patents 4,839,896 and
    //  5,280,488.  See also the paper by Chien, Cunningham, and
    //  Oldham titled "Hybrid Methods for Finding Roots of a Polynomial
    //  With Application to BCH Decoding" published in Transactions on
    //  Information Theory", March 1969 pages 329-335.
    //
    //  I am currently consolidating my notes on how the methods
    //  of fast root finding for cubic and quartic finite field
    //  polynomials were developed.
    //
    //  Note to Neal. ###### Next level - list here the conditions that
    //  result in an uncorrectable error and where each is detected.
    //****************************************************************

    bool success = true;
    int32_t Ln = 4;  // ELP degree is 4 for quartic

    int32_t sigbk[mTParam + 1], b2, b3, b4, b4n, b4d;
    for (int32_t n = 0; n <= Ln; n++)
    {
        sigbk[n] = sigmaN[n];
    }
    //  Test for special case - poly already in correct form
    if (sigbk[1] == 0)
    {
        // Note to Neal.  Re-test extensively without the next decision.
        // Uncorrectable errors caught by this check may be caught by
        // other checks.  Re-testing will require comparing miscorrection
        // rate with and without the decision.
        if (sigbk[3] == 0)
        {
            success = false;
        }
        b2 = sigbk[2];
        b3 = sigbk[3];
        b4 = sigbk[4];
    }
    else
    {
        // ---------- Step b of the Deodhar-Weldon paper ----------
        b4n = ffMult(sigbk[1], sigbk[1]);
        b4d = ffMult(sigbk[3], sigbk[3]) ^ ffMult(sigbk[1], ffMult(sigbk[2], sigbk[3]))
              ^ ffMult(sigbk[4], b4n);
        b4 = ffDiv(b4n, b4d, success);
        b3 = ffMult(sigbk[1], b4);
        b2 = ffMult(b4, ffSquareRoot(ffMult(sigbk[1], sigbk[3])) ^ sigbk[2]);
    }
    // ---------- Step c of the Deodhar-Weldon paper ----------
    //  Set up a cubic and find its 3 roots.
    sigmaN[0] = 1;
    sigmaN[1] = 0;
    sigmaN[2] = b2;
    sigmaN[3] = b3;
    success &= cubicElp(sigmaN);
    int32_t qq = mLoc[1];
    // ---------- Step d of the Deodhar-Weldon paper ----------
    //  Set up a quadratic and find its two roots
    sigmaN[0] = 1;
    sigmaN[1] = ffDiv(b3, qq, success);
    sigmaN[2] = b4;
    success &= quadraticElp(sigmaN);
    int32_t ss = mLoc[0];
    int32_t tt = mLoc[1];
    //  First 2 roots of quartic
    sigmaN[0] = 1;
    sigmaN[1] = qq;
    sigmaN[2] = ss;
    success &= quadraticElp(sigmaN);
    mLoc[2] = mLoc[0];
    mLoc[3] = mLoc[1];
    //  Last 2 roots of quartic
    sigmaN[0] = 1;
    sigmaN[1] = qq;
    sigmaN[2] = tt;
    success &= quadraticElp(sigmaN);
    //  Skip inverse substitution if special case
    if (sigbk[1] != 0)
    {
        // ---------- Step e of the Deodhar-Weldon paper ----------
        //  Do inverse substitution
        int32_t tmp = ffSquareRoot(ffDiv(sigbk[3], sigbk[1], success));
        for (int32_t n = 0; n < 4; n++)
        {
            mLoc[n] = ffInv(mLoc[n], success) ^ tmp;
        }
    }

    return success;
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
bool
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::rootFindChien(int32_t sigmaN[],
                                                                             const int32_t LnOrig)
{
    //****************************************************************
    //  Function: rootFindChien
    //
    //  NOTE: There are seperate rootFind functions for Chien and BTA.
    //
    //  This function is the highest level root finding function.  It calls
    //  lower level root finding functions.  There are special functions
    //  called for error locator polynomial (ELP) degrees 1 to 4 ("m" even)
    //  or 1 to 2 ("m" odd).  If the degree of the ELP is greater than
    //  4 ("m" even) or 2 ("m" odd) the Chien search function is called.
    //****************************************************************
    bool success = true;

    // Note that special cases for quartic and cubic used only for m even
    // while the special cases for linear and quadratic used for m odd or even
    if (LnOrig == 1)
    {
        mLoc[0] = sigmaN[1];  // The root of a degree one error locator polynomial (ELP),
    }
    else if (LnOrig == 2)
    {
        success = quadraticElp(sigmaN);
    }
    else if (LnOrig == 3 && mMOddParam == 0)
    {
        success = cubicElp(sigmaN);
    }
    else if (LnOrig == 4 && mMOddParam == 0)
    {
        success = quarticElp(sigmaN);
    }
    else
    {
        success = chienSearch(sigmaN, LnOrig);
        if (success && mMOddParam == 0)
        {
            // m even - Chien will have divided down to quartic
            success &= quarticElp(sigmaN);
        }
        else if (success && mMOddParam == 1)
        {
            // m odd - Chien will have divided down to quadratic
            success &= quadraticElp(sigmaN);
        }
    }

    return success;
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
bool
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::fixErrors(int32_t Ln)
{
    //****************************************************************
    //  Function: fixErrors
    //
    //  Function to do the actual correction of errors after error
    // locations have been found by the decode function.
    //****************************************************************
    bool success = true;

    memset(mErrLocByte, 0xFFFF, mTParam * sizeof(uint16_t));
    memset(mErrLocBit, 0, mTParam);

    for (int32_t kx = 0; kx < Ln; kx++)
    {
        uint32_t bitLoc = (((mNumCodeWordBytes * 8 - logTable[mLoc[kx]]) - 1) % mNParam);
        // Bounds check fwd displacement because pad bits at end.
        // Note to Neal.
        if (bitLoc < (mNumDataBits + mNumRedundantBits))
        {
            int32_t byteLoc = bitLoc / 8;
            int32_t byteBitNum = 7 - (bitLoc % 8);
            int32_t byteValue = 1;
            for (int32_t jx = byteBitNum; jx >= 1; jx--)
            {
                byteValue *= 2;
            }
            mCodeWord[byteLoc] ^= byteValue;
            mErrLocByte[kx] = byteLoc;
            mErrLocBit[kx] = byteBitNum;
        }
        else
        {
            success = false;
        }
    }

    return success;
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
DecodeStatus
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::bchDecode(void)
{
    //****************************************************************
    //  Function: bchDecode
    //
    //  This function performs decoding by calling -
    //  - a function to compute a remainder
    //  - a function to compute obj->mSyndromes from the remainder
    //  - a Berlekamp-Massey function to compute an error
    //    locator polynomial
    //  - a root finding function to find error locations
    //    (roots of the error locator polynomial)
    //
    //  There is some test code in this function.  If you plan to implement
    //  this function then you may want to remove the test code after
    //  testing is complete.  All such code is commented "For testing only"
    //
    //  Array addresses for syndrome symbols and remainder bytes are passed
    //  to this function but on entry to this function these arrays do not
    //  contain useful data.
    //****************************************************************
    int32_t sigmaN[mTParam + 1];
    bool success = true;
    DecodeStatus status = DecodeStatus::noError;

    memset(mLoc, mLogZVal, ((2 * mTParam) + 1) * sizeof(uint32_t));

    int32_t remainderDetdErr = computeRemainder();

    // If remainderDetdErr is not 0, CW is not err free - could be corr or uncorr
    if (remainderDetdErr != 0)
    {
        // GET HERE IF REMAINDER INDICATES AN ERROR (NON-ZERO REMAINDER)
        status = DecodeStatus::corrected;
        computeSyndromes();

        //  Compute coeff's of ELP using Berlekamp/Massey
        uint32_t Ln = berMas(sigmaN, success);

        if (success)
        {
            //  Find the roots of the ELP
            success &= rootFindChien(sigmaN, Ln);

            if (success)
            {
                //  Fix the errors in the data buffer
                success &= fixErrors(Ln);
            }
        }
    }
    else
    {
        memset(mSyndromes, 0, (2 * mTParam) * sizeof(uint32_t));
    }
    if (!success)
    {
        status = DecodeStatus::uncorrectable;
    }

    return (status);
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
bool
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::encode(
        outpost::Slice<const uint8_t> src_data, outpost::Slice<uint8_t> coded_data)
{
    if (coded_data.getNumberOfElements() < mNandDataSize + mNandSpareSize || !mValid)
    {
        // too small data or invalid template parameter
        return false;
    }

    uint8_t iteration_count = (mNandDataSize * 8) / mNumDataBits;
    uint16_t data_ptr = 0;
    uint16_t chksum_ptr = mNandDataSize;

    /* Convert k size information in bits format */
    for (uint8_t i = 0; i < iteration_count; i++)
    {
        if (((i * mNumDataBytes) + mNumDataBytes) <= src_data.getNumberOfElements())
        {
            // all data fits
            memcpy(mCodeWord, &src_data[i * mNumDataBytes], mNumDataBytes);
        }
        else if ((i * mNumDataBytes) <= src_data.getNumberOfElements())
        {
            // fill up all remaining data
            memset(mCodeWord, NandBCHInterface::fillValue, mNumDataBytes);
        }
        else
        {
            // partial fit and partial not

            // first set all to fill value and then write the one we have there es far as possible
            memset(mCodeWord, NandBCHInterface::fillValue, mNumDataBytes);

            uint32_t sizeRemaining = src_data.getNumberOfElements() - (i * mNumDataBytes);
            memcpy(mCodeWord, &src_data[i * mNumDataBytes], sizeRemaining);
        }

        /* Perform encoding */
        bchEncode();

        /* Copying data incrementally from beginning and checksum at the end */
        memcpy(&(coded_data[0]) + data_ptr, mCodeWord, mNumDataBytes);
        memcpy(&(coded_data[0]) + chksum_ptr, &mCodeWord[mNumDataBytes], mNumRedundantBytes);
        data_ptr += mNumDataBytes;
        chksum_ptr += (mNumRedundantBytes);
        memset(mCodeWord, 0, mNumCodeWordBytes);
    }

    return true;
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
DecodeStatus
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::decode(
        outpost::Slice<const uint8_t> coded_data, outpost::Slice<uint8_t> src_data)
{
    if (coded_data.getNumberOfElements() < mNandDataSize + mNandSpareSize || !mValid)
    {
        // too small data or invalid template parameter
        return DecodeStatus::invalidParameters;
    }

    DecodeStatus status = DecodeStatus::noError;

    uint8_t iteration_count = (mNandDataSize * 8) / mNumDataBits;

    /* Convert k size information in bits format */
    for (uint16_t i = 0; i < iteration_count; i++)
    {
        /* Retrieving data incrementally from beginning and checksum at the end */
        memcpy(mCodeWord, &coded_data[i * mNumDataBytes], mNumDataBytes);
        memcpy(&mCodeWord[mNumDataBytes],
               &coded_data[i * (mNumRedundantBytes) + mNandDataSize],
               (mNumRedundantBytes));

        /* Perform decoding */
        status = combine(status, bchDecode());

        // Copy anyways, we tell them whether it is correct or not
        if (src_data.getNumberOfElements() >= ((i + 1) * mNumDataBytes))
        {
            // all data fitting
            memcpy(&src_data[0] + (i * mNumDataBytes), mCodeWord, mNumDataBytes);
        }
        else if (src_data.getNumberOfElements() <= (i * mNumDataBytes))
        {
            // do nothing but still go on so that errors are found that are positioned later
        }
        else
        {
            // partial
            uint32_t sizeRemaining = src_data.getNumberOfElements() - (i * mNumDataBytes);
            memcpy(&src_data[0] + (i * mNumDataBytes), mCodeWord, sizeRemaining);
        }
    }
    return status;
}

template <uint32_t mMParam, uint32_t mTParam, uint32_t mNandDataSize, uint32_t mNandSpareSize>
bool
NandBCHCTime<mMParam, mTParam, mNandDataSize, mNandSpareSize>::isChecksumEmpty(uint8_t* buffer)
{
    bool result = true;
    uint8_t iteration_count = (mNandDataSize * 8) / mNumDataBits;
    uint32_t codeSize = (iteration_count * mNumRedundantBytes);

    /* Convert k size information in bits format */
    for (uint32_t i = mNandDataSize; i < mNandDataSize + codeSize; i++)
    {
        if (buffer[i] != 0xFF)
        {
            result = false;
            break;
        }
    }

    return result;
}

}  // namespace utils
}  // namespace outpost
#endif
#endif  // Version check
