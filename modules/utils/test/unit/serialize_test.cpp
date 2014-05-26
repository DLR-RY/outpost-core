
#include <cobc/utils/serialize.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace cobc;

TEST(SerialzeTest, store)
{
	uint8_t data[16] = { 0, };

	EXPECT_EQ(0, data[0]);

	Serialize payload(data);

	payload.store8(0xab);

	EXPECT_EQ(0xab, data[0]);

	payload.store16(0xef12);

	EXPECT_EQ(0xef, data[1]);
	EXPECT_EQ(0x12, data[2]);

	payload.store24(0xa6c01a);

	EXPECT_EQ(0xa6, data[3]);
	EXPECT_EQ(0xc0, data[4]);
	EXPECT_EQ(0x1a, data[5]);

	payload.store32(0x61a9f51e);

	EXPECT_EQ(0x61, data[6]);
	EXPECT_EQ(0xa9, data[7]);
	EXPECT_EQ(0xf5, data[8]);
	EXPECT_EQ(0x1e, data[9]);
}

TEST(SerializeTest, storeFloat)
{
	float f = 3.14159f;

	uint8_t data[4];

	Serialize payload(data);

	payload.storeFloat(f);

	EXPECT_EQ(0x40, data[0]);
	EXPECT_EQ(0x49, data[1]);
	EXPECT_EQ(0x0F, data[2]);
	EXPECT_EQ(0xD0, data[3]);

	EXPECT_EQ(4, payload.getPosition());
}

TEST(SerializeTest, storeDouble)
{
	double d = 3.1415926535897931;

	uint8_t data[8];

	Serialize payload(data);

	payload.storeDouble(d);

	EXPECT_EQ(0x40, data[0]);
	EXPECT_EQ(0x09, data[1]);
	EXPECT_EQ(0x21, data[2]);
	EXPECT_EQ(0xFB, data[3]);
	EXPECT_EQ(0x54, data[4]);
	EXPECT_EQ(0x44, data[5]);
	EXPECT_EQ(0x2D, data[6]);
	EXPECT_EQ(0x18, data[7]);

	EXPECT_EQ(8, payload.getPosition());
}

TEST(DeserialzeTest, getPosition)
{
	uint8_t data[32] = {};

	Deserialize payload(data);

	EXPECT_EQ(0, payload.getPosition());

	payload.read8();
	EXPECT_EQ(1, payload.getPosition());

	payload.read16();
	EXPECT_EQ(3, payload.getPosition());

	payload.read8();
	EXPECT_EQ(4, payload.getPosition());

	payload.read32();
	EXPECT_EQ(8, payload.getPosition());

	payload.read8();
	EXPECT_EQ(9, payload.getPosition());

	payload.read32();
	EXPECT_EQ(13, payload.getPosition());

	payload.readFloat();
	EXPECT_EQ(17, payload.getPosition());

	payload.readDouble();
	EXPECT_EQ(25, payload.getPosition());
}

TEST(DeserialzeTest, peek)
{
	uint8_t data[18] = {
		0xAB,
		0xEF, 0x12,
		0xA6, 0xC0, 0x1A,
		0x61, 0xA9, 0xF5, 0x1E,
		0xAE, 0x5F, 0xA2, 0xB0, 0x00, 0x7D, 0x32, 0xC2
	};

	Deserialize payload(data);

	uint8_t d8 = payload.read8();
	EXPECT_EQ(0xAB, d8);
	EXPECT_EQ(1, payload.getPosition());

	uint16_t d16 = payload.read16();
	EXPECT_EQ(0xEF12, d16);
	EXPECT_EQ(3, payload.getPosition());

	uint32_t d24 = payload.read24();
	EXPECT_EQ(0xA6C01AU, d24);
	EXPECT_EQ(6, payload.getPosition());

	uint32_t d32 = payload.read32();
	EXPECT_EQ(0x61A9F51EUL, d32);
	EXPECT_EQ(10, payload.getPosition());

	uint64_t d64 = payload.read64();
	EXPECT_EQ(0xAE5FA2B0007D32C2ULL, d64);
	EXPECT_EQ(18, payload.getPosition());
}

TEST(DeserialzeTest, peekFloat)
{
	uint8_t data[4] = {
		0x40, 0x49, 0x0F, 0xD0
	};

	Deserialize payload(data);

	float f = payload.peekFloat(0);

	EXPECT_FLOAT_EQ(3.14159f, f);
	EXPECT_EQ(0, payload.getPosition());
}

TEST(DeserialzeTest, peekDouble)
{
	uint8_t data[8] = {
		0x40, 0x09, 0x21, 0xFB, 0x54, 0x44, 0x2D, 0x18
	};

	Deserialize payload(data);

	double d = payload.peekDouble(0);

	EXPECT_DOUBLE_EQ(3.1415926535897931, d);
	EXPECT_EQ(0, payload.getPosition());
}

TEST(DeserialzeTest, read)
{
	uint8_t data[18] = {
		0xAB,
		0xEF, 0x12,
		0xA6, 0xC0, 0x1A,
		0x61, 0xA9, 0xF5, 0x1E,
		0xAE, 0x5F, 0xA2, 0xB0, 0x00, 0x7D, 0x32, 0xC2
	};

	Deserialize payload(data);

	uint8_t d8 = payload.peek8(0);
	EXPECT_EQ(0xAB, d8);

	uint16_t d16 = payload.peek16(1);
	EXPECT_EQ(0xEF12, d16);

	uint32_t d24 = payload.peek24(3);
	EXPECT_EQ(0xA6C01AU, d24);

	uint32_t d32 = payload.peek32(6);
	EXPECT_EQ(0x61A9F51EUL, d32);

	uint64_t d64 = payload.peek64(10);
	EXPECT_EQ(0xAE5FA2B0007D32C2ULL, d64);
}

TEST(DeserialzeTest, readFloat)
{
	uint8_t data[4] = {
		0x40, 0x49, 0x0F, 0xD0
	};

	Deserialize payload(data);

	float f = payload.readFloat();

	EXPECT_FLOAT_EQ(3.14159f, f);
	EXPECT_EQ(4, payload.getPosition());
}

TEST(DeserialzeTest, readDouble)
{
	uint8_t data[8] = {
		0x40, 0x09, 0x21, 0xFB, 0x54, 0x44, 0x2D, 0x18
	};

	Deserialize payload(data);

	double d = payload.readDouble();

	EXPECT_DOUBLE_EQ(3.1415926535897931, d);
	EXPECT_EQ(8, payload.getPosition());
}
