
#include <cobc/utils/cobs.h>

#include <string.h>     // for memset
#include <unittest/harness.h>

using cobc::utils::CobsEncoder;

class CobsEncoderRoundtriptTest : public ::testing::Test
{
public:
    virtual void
    SetUp()
    {
        memset(encoded, 0xAB, sizeof(encoded));
        memset(actual,  0xAB, sizeof(actual));
    }

    size_t
    roundtrip(const uint8_t* input,
              size_t inputLength)
    {
        size_t encodedLength = CobsEncoder::encode(input, inputLength, encoded, sizeof(encoded));
        size_t decodedLength = CobsEncoder::decode(encoded, encodedLength, actual);

        return decodedLength;
    }

    uint8_t encoded[1024];
    uint8_t actual[1024];
};

TEST_F(CobsEncoderRoundtriptTest, singleZeroByte)
{
    uint8_t input[1] = { 0 };

    uint8_t expected[3] = { 0, 0xAB, 0xAB };

    size_t decodedLength = roundtrip(input, sizeof(input));

    EXPECT_EQ(sizeof(input), decodedLength);
    EXPECT_THAT(expected, ::testing::ElementsAreArray(actual, 3));
}

TEST_F(CobsEncoderRoundtriptTest, zeroPrefixAndSuffix)
{
    uint8_t input[3] = { 0, 1, 0 };

    uint8_t expected[5] = { 0, 1, 0, 0xAB, 0xAB };

    size_t decodedLength = roundtrip(input, sizeof(input));

    EXPECT_EQ(sizeof(input), decodedLength);
    EXPECT_THAT(expected, ::testing::ElementsAreArray(actual, 5));
}

TEST_F(CobsEncoderRoundtriptTest, blockOfDataWithoutZero)
{
    uint8_t input[512];
    for (size_t i = 0; i < 512; ++i)
    {
        input[i] = (i % 255) + 1;
    }

    size_t decodedLength = roundtrip(input, sizeof(input));

    EXPECT_EQ(sizeof(input), decodedLength);
    EXPECT_THAT(input, ::testing::ElementsAreArray(actual, sizeof(input)));
}

