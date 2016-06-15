
#ifndef HC_SR04_HC_SR04_H
#define HC_SR04_HC_SR04_H

#include "mbed-drivers/mbed.h"


class HC_SR04 {

private:

    DigitalOut _trigger;
    InterruptIn _ping;
    timestamp_t _timeout_us;
    Timeout _timeout;
    Timer _timer;
    enum class State {IDLE, LISTENING, MEASURING} _state;
    int _latest_measurement_mm;

    mbed::util::FunctionPointer1<void, int> _measurement_callback;
    mbed::util::FunctionPointer0<void> _timeout_callback;


    void handle_timeout(void);
    void ping_rise(void);
    void ping_fall(void);

public:

    HC_SR04(PinName trigger_pin, PinName reply_pin) :
        _trigger(trigger_pin),
        _ping(reply_pin),
        _timeout_us(0),
        _state(State::IDLE),
        _latest_measurement_mm(0) {
            _ping.rise(this, &HC_SR04::ping_rise);
            _ping.fall(this, &HC_SR04::ping_fall);
        };

    void set_measurement_callback(void(*fptr)(int distance_mm)){
        _measurement_callback.attach(fptr);
    }

    template<typename T>
    void set_measurement_callback(T* tptr, void (T::*mptr)(int)) {
        _measurement_callback.attach(tptr, mptr);
    }

    void set_timeout_callback(void(*fptr)(void)){
        _timeout_callback.attach(fptr);
    }

    template<typename T>
    void set_timeout_callback(T* tptr, void (T::*mptr)(void)) {
        _timeout_callback.attach(tptr, mptr);
    }

    void set_timeout_us(int us) {
        _timeout_us = us;
    }

    void set_timeout_mm(int mm) {
        _timeout_us = 250 + (3 * (mm * 29)) / 10;
    }

    void trigger_ping(void);

};

#endif // HC_SR04_HC_SR04_H

