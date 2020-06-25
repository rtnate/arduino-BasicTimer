#ifndef _BASIC_TIMER_H_
#define _BASIC_TIMER_H_

#include <Arduino.h>

/**
 * @brief Class that wraps millis() based timers for easier use.
 * 
 * @tparam timer_t The storage class for timer values
 * 
 *  A TBasicTimer makes it easy to perform an action after a certain amount of milliseconds has passed.
 */
template <class timer_t = uint32_t>
class TBasicTimer
{
    public:
        /**
         * @brief onExpire callback handler type
         */
        typedef void(*OnExpireFunction)();

        /**
         * @brief Construct a new TBasicTimer 
         * 
         * @param timeout The timers timeout in milliseconds
         */
        TBasicTimer(timer_t timeout): _value(0), _timeout(timeout){};

        /**
         * @brief Resets the timer (so that it is no longer expired)
         */
        void reset(){ _value = now(); };

        /**
         * @brief Check if the timer has expired
         * 
         * @return true If the timer has expired
         * @return false If it has yet to expire
         */
        bool hasExpired() 
        {
            auto current = now();
            if (current - _value > _timeout) return true;
            else return false;
        }

        /**
         * @brief Get the current time in milliseconds
         * 
         * @return timer_t The current time in milliseconds
         */
        timer_t now()
        {
            return millis();
        }

        /**
         * @brief Set the timer's timeout to the supplied value
         * 
         * @param timeout The new timer period in milliseconds
         */
        void setTimeout(timer_t timeout){ _timeout = timeout; };

        /**
         * @brief Runs the timer, executing the supplied function if the timer has expired.
         * 
         * @param callback The function to execute
         * @param resetOnExpire If true, the timer will reset every time (executing callback after every timeout period)
         */
        void whenExpired(OnExpireFunction callback, bool resetOnExpire = false)
        {
            if(hasExpired())
            {
                if (resetOnExpire) reset();
                callback();
            }
        }
    protected:
        timer_t _value; //!< The last timestamp at which the timer was reset
        timer_t _timeout;//!< The timeout value in milliseconds
};

/**
 * @brief Template class for a timer with a static, constant Timeout value
 * 
 * @tparam TIMEOUT The timeout in milliseconds
 * @tparam timer_t The timer storage class 
 */
template <unsigned long TIMEOUT, class timer_t = uint32_t>
class TStaticTimer
{
    public:
        /**
         * @brief Construct a new TStaticTimer object
         * 
         */
        TStaticTimer(): _value(0){};

        /**
         * @brief Reset the timer, setting its value to the current time
         * 
         */
        void reset(){ _value = now(); };

        /**
         * @brief Checks it the timer has expired
         * 
         * @return true If the timeout has passed
         * @return false If not.
         */
        bool hasExpired() 
        {
            auto current = millis();
            if (current - _value > TIMEOUT) return true;
            else return false;
        }

        /**
         * @brief Gets the currenet timestamp in milliseconds
         * 
         * @return timer_t The current timestamp in milliseconds
         */
        timer_t now()
        {
            return millis();
        }

        /**
         * @brief Runs the timer, executing the supplied function if the timer has expired.
         * 
         * @param callback The function to execute
         * @param resetOnExpire If true, the timer will reset every time (executing callback after every timeout period)
         */
        void whenExpired(OnExpireFunction callback, bool resetOnExpire = false)
        {
            if(hasExpired())
            {
                if (resetOnExpire) reset();
                callback();
            }
        }
    protected:
        timer_t _value;
};

/**
 * @brief Template Class for A Basic Timer That Provides a callback on expire
 * 
 * @tparam timer_t - The timer storage class
 */
template <class timer_t = uint32_t>
class TCallbackTimer: public TBasicTimer<timer_t>
{
    public:
        

        /**
         * @brief Construct a new TCallbackTimer object with the provided timeout
         * 
         * @param timeout The timeout in milliseconds
         */
        TCallbackTimer(timer_t timeout): TBasicTimer<timer_t>(timeout), expiredCallback(nullptr){};

        /**
         * @brief Construct a new TCallbackTimer object with the provided timeout and callback
         * 
         * @param timeout The timeout in milliseconds
         * @param callback The onExpire callback function
         */
        TCallbackTimer(timer_t timeout, OnExpireFunction callback): TBasicTimer<timer_t>(timeout), expiredCallback(callback){};

        /**
         * @brief Assign a callback function to be called on time expire
         * 
         * @param callback The onExpire callback function
         */
        void onExpire(OnExpireFunction callback){ expiredCallback = callback; }

        /**
         * @brief Run the timer.  
         * 
         * @param reset Resets the timer each time it expires for continuous execution 
         * 
         * This function should be called from the main loop to dispatch callbacks on expire
         */
        void run(bool reset = true)
        {
            if (this->hasExpired())
            {
                if (reset) this->reset();
                if (expiredCallback != nullptr) this->expiredCallback();
            }
        }
    protected:
        /**
         * @brief On expire callback pointer storage
         */
        OnExpireFunction expiredCallback;
};

/**
 * @brief BasicTimer class that wraps millis() based timers for easier use.
 * 
 *  A BasicTimer makes it easy to perform an action after a certain amount of milliseconds has passed.
 *  BasicTimer uses uint32_t values for internal storage.
 */
typedef TBasicTimer<> BasicTimer;

/**
 * @brief StaticTimer is like a BasicTimer, but with a fixed, constant timeout value
 * 
 * @see BasicTimer
 * @tparam TIMEOUT The fixed timeout in milliseconds
 * 
 * Using a static timer means the compiler can optimize the timeout calculations, and
 * timer objects will occupy less memory.
 */
template<unsigned long TIMEOUT>
using StaticTimer = TStaticTimer<TIMEOUT>;

/**
 * @brief CallbackTimer is like a BasicTimer, but it stores a callback function to be executed on timeout.
 * 
 * @see BasicTimer
 * 
 * Using a callback timer allows for easily changing the action of the timer at run time.
 */
typedef TCallbackTimer<> CallbackTimer;

#endif