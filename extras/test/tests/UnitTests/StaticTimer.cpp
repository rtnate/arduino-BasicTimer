#include <gtest/gtest.h>
#include <BasicTimer.h>

TEST(StaticTimerUnitTests, TestInstantiation) {
    //Default Construction
    StaticTimer<550> staticTimer;
    EXPECT_EQ(staticTimer.timeout(), 550);
    //Large Size Timeout Construction 
    StaticTimer<35000000> largeTimer;
    EXPECT_EQ(largeTimer.timeout(), 35000000);
    //Zero Timeout Construction
    StaticTimer<0> zeroTimeout;
    EXPECT_EQ(zeroTimeout.timeout(), 0);
}


TEST(StaticTimerUnitTests, TestMockTimeNow) {
    auto mockTime = MockArduino::FakeTime::GetInstance();
    mockTime->reset();
    //Default Construction
    StaticTimer<650> staticTimer;
    ASSERT_EQ(staticTimer.timeout(), 650);
    ASSERT_EQ(staticTimer.now(), 0);
    mockTime->advanceMillis(500);
    EXPECT_EQ(staticTimer.now(), 500);
    mockTime->advanceMillis(51);
    EXPECT_EQ(staticTimer.now(), 551);
    mockTime->advanceMillis(3000008);
    EXPECT_EQ(staticTimer.now(), 3000559);
}

TEST(StaticTimerUnitTests, TestHasExpired) {
    auto mockTime = MockArduino::FakeTime::GetInstance();
    mockTime->reset();
    //Default Construction
    StaticTimer<500> staticTimer;
    staticTimer.begin();
    ASSERT_EQ(staticTimer.timeout(), 500);
    ASSERT_EQ(staticTimer.now(), 0);
    EXPECT_FALSE(staticTimer.hasExpired());
    mockTime->advanceMillis(100);
    EXPECT_FALSE(staticTimer.hasExpired());
    mockTime->advanceMillis(400);
    EXPECT_FALSE(staticTimer.hasExpired());
    mockTime->advanceMillis(1);
    EXPECT_TRUE(staticTimer.hasExpired());
}

TEST(StaticTimerUnitTests, TestReset) {
    auto mockTime = MockArduino::FakeTime::GetInstance();
    mockTime->reset();
    //Default Construction
    StaticTimer<800> staticTimer;
    staticTimer.begin();
    ASSERT_EQ(staticTimer.timeout(), 800);
    ASSERT_EQ(staticTimer.now(), 0);
    EXPECT_FALSE(staticTimer.hasExpired());
    mockTime->advanceMillis(801);
    EXPECT_TRUE(staticTimer.hasExpired());
    mockTime->advanceMillis(100);
    EXPECT_TRUE(staticTimer.hasExpired());
    staticTimer.reset();
    EXPECT_FALSE(staticTimer.hasExpired());
    mockTime->advanceMillis(500);
    EXPECT_FALSE(staticTimer.hasExpired());
    mockTime->advanceMillis(300);
    EXPECT_FALSE(staticTimer.hasExpired());
    mockTime->advanceMillis(1);
    EXPECT_TRUE(staticTimer.hasExpired());
}

TEST(StaticTimerUnitTests, TestMultipleExpireReset) {
    auto mockTime = MockArduino::FakeTime::GetInstance();
    mockTime->reset();
    //Construct Instant
    StaticTimer<788> staticTimer;
    staticTimer.begin();
    ASSERT_EQ(staticTimer.timeout(), 788);
    ASSERT_EQ(staticTimer.now(), 0);

    // Run this a bunch of times
    for (int i = 0; i < 10000; i++) {
        EXPECT_FALSE(staticTimer.hasExpired());
        mockTime->advanceMillis(788);
        EXPECT_FALSE(staticTimer.hasExpired());
        mockTime->advanceMillis(1);
        EXPECT_TRUE(staticTimer.hasExpired());
        staticTimer.reset();
        EXPECT_FALSE(staticTimer.hasExpired());
    }
}


