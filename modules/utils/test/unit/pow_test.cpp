
#include <stdint.h>

#include <unittest/harness.h>

#include <cobc/utils/pow.h>

using namespace cobc;

TEST(PowerOfTwoTest, test)
{
	EXPECT_EQ(cobc::PowerOfTwo<3>::value, 8);
	EXPECT_EQ(cobc::PowerOfTwo<24>::value, 256*256*256);
}

TEST(PowTest, test)
{
	int32_t value;
	
	value = cobc::Pow<2, 3>::value;
	EXPECT_EQ(value, 8);
	
	value = cobc::Pow<3, 4>::value;
	EXPECT_EQ(value, 81);
}
