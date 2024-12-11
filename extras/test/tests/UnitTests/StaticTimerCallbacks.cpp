#include <gtest/gtest.h>
#include <BasicTimer.h>

class StaticTimerCallbackUnitTests: public testing::Test {
    protected:
        void SetUp() override {
            GetTime()->reset();
            cbInstance_ = this;
            Reset();
        };

        MockArduino::FakeTime* GetTime() {
            return MockArduino::FakeTime::GetInstance();
        }
        
        static void TimerCallback() {
            if (cbInstance_ != nullptr) {
                cbInstance_->onCallback();
            }
        }

        static void TimerCallbackWithPtr(testing::Test* testPtr) {
            if (cbInstance_ != nullptr) {
                cbInstance_->onCallback();
                cbInstance_->lastCallbackArg_ = testPtr;
            }
        }

        static void TimerCallbackWithIntArg(int myArg) {
            if (cbInstance_ != nullptr) {
                cbInstance_->onCallback();
                cbInstance_->lastCallbackIntArg_ = myArg;
            }
        }

        void onCallback() {
            callbackInvoked_ = true;
            callbackInvokedCount_++;
            lastCallbackTimestamp_ = GetTime()->millis();
        }

        int CallbackInvokedCount() const {
            return callbackInvokedCount_;
        }

        bool CallbackWasInvoked() const {
            return callbackInvoked_;
        }

        uint32_t LastCallbackTimestamp() const {
            return lastCallbackTimestamp_;
        }

        int LastCallbackIntArgument() const {
            return lastCallbackIntArg_;
        }

        testing::Test* LastCallbackPointerArgument() const {
            return lastCallbackArg_;
        }

        void Reset() {
            callbackInvokedCount_ = 0;
            callbackInvoked_ = false;
            lastCallbackTimestamp_ = 0;
            lastCallbackArg_ = nullptr;
            lastCallbackIntArg_ = -1;
        }
    private:
        static StaticTimerCallbackUnitTests* cbInstance_;
        MockArduino::FakeTime* fakeTime_;
        std::atomic_bool callbackInvoked_;
        std::atomic_int callbackInvokedCount_;
        std::atomic_uint32_t lastCallbackTimestamp_;
        std::atomic_uintptr_t lastPtrArg_;
        testing::Test* lastCallbackArg_;
        int lastCallbackIntArg_;
};

StaticTimerCallbackUnitTests* StaticTimerCallbackUnitTests::cbInstance_ = nullptr;

TEST_F(StaticTimerCallbackUnitTests, TestCallbackInvokable) {
    ASSERT_EQ(CallbackInvokedCount(), 0);
    ASSERT_FALSE(CallbackWasInvoked());
    ASSERT_EQ(LastCallbackTimestamp(), 0);
    GetTime()->advanceMillis(500);
    TimerCallback();
    EXPECT_EQ(CallbackInvokedCount(), 1);
    EXPECT_TRUE(CallbackWasInvoked());
    EXPECT_EQ(LastCallbackTimestamp(), 500);
}

TEST_F(StaticTimerCallbackUnitTests, TestBasicTimerExpireCallback) {
    ASSERT_EQ(CallbackInvokedCount(), 0);
    ASSERT_FALSE(CallbackWasInvoked());
    ASSERT_EQ(LastCallbackTimestamp(), 0);

    StaticTimer<550> timer;
    ASSERT_EQ(timer.timeout(), 550);
    ASSERT_FALSE(timer.hasExpired());
    
    for(int i = 0; i < 600; i++) {
        GetTime()->advanceMillis(1);
        timer.whenExpired(TimerCallback);
    }
    EXPECT_EQ(CallbackInvokedCount(), 1);
    EXPECT_TRUE(CallbackWasInvoked());
    EXPECT_EQ(LastCallbackTimestamp(), 551);
}

TEST_F(StaticTimerCallbackUnitTests, TestBasicTimerMultipleExpireCallback) {
    ASSERT_EQ(CallbackInvokedCount(), 0);
    ASSERT_FALSE(CallbackWasInvoked());
    ASSERT_EQ(LastCallbackTimestamp(), 0);

    StaticTimer<500> timer;
    ASSERT_EQ(timer.timeout(), 500);
    ASSERT_FALSE(timer.hasExpired());
    
    for(int i = 0; i < 1200; i++) {
        GetTime()->advanceMillis(1);
        timer.whenExpired(TimerCallback);
    }
    EXPECT_EQ(CallbackInvokedCount(), 2);
    EXPECT_TRUE(CallbackWasInvoked());
    EXPECT_EQ(LastCallbackTimestamp(), 1002);
}

TEST_F(StaticTimerCallbackUnitTests, TestBasicTimerExpireCallbackWithIntArgument) {
    ASSERT_EQ(CallbackInvokedCount(), 0);
    ASSERT_FALSE(CallbackWasInvoked());
    ASSERT_EQ(LastCallbackTimestamp(), 0);
    ASSERT_EQ(LastCallbackIntArgument(), -1);

    StaticTimer<800> timer;
    ASSERT_EQ(timer.timeout(), 800);
    ASSERT_FALSE(timer.hasExpired());
    
    for(int i = 0; i < 1000; i++) {
        GetTime()->advanceMillis(1);
        timer.whenExpired(TimerCallbackWithIntArg, 787);
    }
    EXPECT_EQ(CallbackInvokedCount(), 1);
    EXPECT_TRUE(CallbackWasInvoked());
    EXPECT_EQ(LastCallbackTimestamp(), 801);
    EXPECT_EQ(LastCallbackIntArgument(), 787);
    for(int i = 0; i < 1000; i++) {
        GetTime()->advanceMillis(1);
        timer.whenExpired(TimerCallbackWithIntArg, 11);
    }
    EXPECT_EQ(CallbackInvokedCount(), 2);
    EXPECT_TRUE(CallbackWasInvoked());
    EXPECT_EQ(LastCallbackTimestamp(), 1602);
    EXPECT_EQ(LastCallbackIntArgument(), 11);
}


TEST_F(StaticTimerCallbackUnitTests, TestBasicTimerExpireCallbackWithPointerArgument) {
    ASSERT_EQ(CallbackInvokedCount(), 0);
    ASSERT_FALSE(CallbackWasInvoked());
    ASSERT_EQ(LastCallbackTimestamp(), 0);
    ASSERT_EQ(LastCallbackIntArgument(), -1);

    StaticTimer<999> timer;
    ASSERT_EQ(timer.timeout(), 999);
    ASSERT_FALSE(timer.hasExpired());
    
    for(int i = 0; i < 1100; i++) {
        GetTime()->advanceMillis(1);
        timer.whenExpired(TimerCallbackWithPtr, (testing::Test*)this);
    }
    EXPECT_EQ(CallbackInvokedCount(), 1);
    EXPECT_TRUE(CallbackWasInvoked());
    EXPECT_EQ(LastCallbackTimestamp(), 1000);
    EXPECT_EQ(LastCallbackPointerArgument(), this);
}