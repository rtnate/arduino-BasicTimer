#include <gtest/gtest.h>
#include <BasicTimer.h>

TEST(BasicTimerUnitTests, TestInstantiation) {
    //Default Construction
    BasicTimer basicTimer;
    EXPECT_EQ(basicTimer.timeout(), 500);
    //Timeout Construction
    BasicTimer timeoutTimer(800);
    EXPECT_EQ(timeoutTimer.timeout(), 800);
    //Large Size Timeout Construction 
    BasicTimer largeTimer(35000000);
    EXPECT_EQ(largeTimer.timeout(), 35000000);
    //Zero Timeout Construction
    BasicTimer zeroTimeout(0);
    EXPECT_EQ(zeroTimeout.timeout(), 0);
}

TEST(BasicTimerUnitTests, TestSetTimeout) {
    //Default Construction
    BasicTimer basicTimer;
    ASSERT_EQ(basicTimer.timeout(), 500);
    //Set Timeout
    basicTimer.setTimeout(799);
    EXPECT_EQ(basicTimer.timeout(), 799);
    basicTimer.setTimeout(39999000);
    EXPECT_EQ(basicTimer.timeout(), 39999000);
    basicTimer.setTimeout(0);
    EXPECT_EQ(basicTimer.timeout(), 0);
}

TEST(BasicTimerUnitTests, TestMockTimeNow) {
    auto mockTime = MockArduino::FakeTime::GetInstance();
    mockTime->reset();
    //Default Construction
    BasicTimer basicTimer;
    ASSERT_EQ(basicTimer.timeout(), 500);
    ASSERT_EQ(basicTimer.now(), 0);
    mockTime->advanceMillis(500);
    EXPECT_EQ(basicTimer.now(), 500);
    mockTime->advanceMillis(51);
    EXPECT_EQ(basicTimer.now(), 551);
    mockTime->advanceMillis(3000008);
    EXPECT_EQ(basicTimer.now(), 3000559);
}

TEST(BasicTimerUnitTests, TestHasExpired) {
    auto mockTime = MockArduino::FakeTime::GetInstance();
    mockTime->reset();
    //Default Construction
    BasicTimer basicTimer;
    basicTimer.begin();
    ASSERT_EQ(basicTimer.timeout(), 500);
    ASSERT_EQ(basicTimer.now(), 0);
    EXPECT_FALSE(basicTimer.hasExpired());
    mockTime->advanceMillis(100);
    EXPECT_FALSE(basicTimer.hasExpired());
    mockTime->advanceMillis(400);
    EXPECT_FALSE(basicTimer.hasExpired());
    mockTime->advanceMillis(1);
    EXPECT_TRUE(basicTimer.hasExpired());
}

TEST(BasicTimerUnitTests, TestReset) {
    auto mockTime = MockArduino::FakeTime::GetInstance();
    mockTime->reset();
    //Default Construction
    BasicTimer basicTimer;
    basicTimer.begin();
    ASSERT_EQ(basicTimer.timeout(), 500);
    ASSERT_EQ(basicTimer.now(), 0);
    EXPECT_FALSE(basicTimer.hasExpired());
    mockTime->advanceMillis(501);
    EXPECT_TRUE(basicTimer.hasExpired());
    mockTime->advanceMillis(100);
    EXPECT_TRUE(basicTimer.hasExpired());
    basicTimer.reset();
    EXPECT_FALSE(basicTimer.hasExpired());
    mockTime->advanceMillis(300);
    EXPECT_FALSE(basicTimer.hasExpired());
    mockTime->advanceMillis(200);
    EXPECT_FALSE(basicTimer.hasExpired());
    mockTime->advanceMillis(1);
    EXPECT_TRUE(basicTimer.hasExpired());
}

TEST(BasicTimerUnitTests, TestMultipleExpireReset) {
    auto mockTime = MockArduino::FakeTime::GetInstance();
    mockTime->reset();
    //Construct Instant
    BasicTimer basicTimer(788);
    basicTimer.begin();
    ASSERT_EQ(basicTimer.timeout(), 788);
    ASSERT_EQ(basicTimer.now(), 0);

    // Run this a bunch of times
    for (int i = 0; i < 10000; i++) {
        EXPECT_FALSE(basicTimer.hasExpired());
        mockTime->advanceMillis(788);
        EXPECT_FALSE(basicTimer.hasExpired());
        mockTime->advanceMillis(1);
        EXPECT_TRUE(basicTimer.hasExpired());
        basicTimer.reset();
        EXPECT_FALSE(basicTimer.hasExpired());
    }
}

TEST(BasicTimerUnitTests, TestVariableTimeoutNormalFlow)
{
    auto mockTime = MockArduino::FakeTime::GetInstance();
    mockTime->reset();
    //Default Construction
    BasicTimer basicTimer;
    basicTimer.begin();
    ASSERT_EQ(basicTimer.timeout(), 500);
    ASSERT_EQ(basicTimer.now(), 0);

    //Test Expire at 500 
    EXPECT_FALSE(basicTimer.hasExpired());
    mockTime->advanceMillis(500);
    EXPECT_FALSE(basicTimer.hasExpired());
    mockTime->advanceMillis(1);
    EXPECT_TRUE(basicTimer.hasExpired());
    basicTimer.setTimeout(1120);
    basicTimer.reset();
    EXPECT_FALSE(basicTimer.hasExpired());
    mockTime->advanceMillis(501);
    EXPECT_FALSE(basicTimer.hasExpired());
    mockTime->advanceMillis(1120-501);
    EXPECT_FALSE(basicTimer.hasExpired());
    mockTime->advanceMillis(1);
    EXPECT_TRUE(basicTimer.hasExpired());
    //Change timeout to large value
    basicTimer.setTimeout(11223344);
    basicTimer.reset();
    EXPECT_FALSE(basicTimer.hasExpired());
    mockTime->advanceMillis(1121);
    EXPECT_FALSE(basicTimer.hasExpired());
    mockTime->advanceMillis(11223344-1121);
    EXPECT_FALSE(basicTimer.hasExpired());
    mockTime->advanceMillis(1);
    EXPECT_TRUE(basicTimer.hasExpired());
}

TEST(BasicTimerUnitTests, TestVariableTimeoutUnusualFlow) 
{
    auto mockTime = MockArduino::FakeTime::GetInstance();
    mockTime->reset();
    //Default Construction
    BasicTimer basicTimer;
    basicTimer.begin();
    ASSERT_EQ(basicTimer.timeout(), 500);
    ASSERT_EQ(basicTimer.now(), 0);

    EXPECT_FALSE(basicTimer.hasExpired());
    mockTime->advanceMillis(500);
    EXPECT_FALSE(basicTimer.hasExpired());
    mockTime->advanceMillis(1);
    EXPECT_TRUE(basicTimer.hasExpired());
    basicTimer.setTimeout(1150);
    EXPECT_FALSE(basicTimer.hasExpired());
    mockTime->advanceMillis(1150-501);
    EXPECT_FALSE(basicTimer.hasExpired());
    mockTime->advanceMillis(1);
    EXPECT_TRUE(basicTimer.hasExpired());
    basicTimer.setTimeout(9100);
    EXPECT_FALSE(basicTimer.hasExpired());
    mockTime->advanceMillis(9100-1151);
    EXPECT_FALSE(basicTimer.hasExpired());
    mockTime->advanceMillis(1);
    EXPECT_TRUE(basicTimer.hasExpired());
}
