
#include <unittest/harness.h>
#include <cobc/time/duration.h>

using namespace cobc::time;

TEST(DurationTest, derivedTypes)
{
    ASSERT_EQ(Seconds(5), Milliseconds(5000));
    ASSERT_EQ(Microseconds(1000), Milliseconds(1));
    
    Duration duration = Seconds(5);
    
    ASSERT_EQ(5, duration.seconds());
    ASSERT_EQ(5000, duration.milliseconds());
    ASSERT_EQ(5000000, duration.microseconds());
}
