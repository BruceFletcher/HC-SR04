
#include "hc-sr04/hc-sr04.h"

void HC_SR04::handle_timeout(void) {
    _state = State::IDLE;

    if (_timeout_callback) {
        _timeout_callback.call();
    }
}

void HC_SR04::ping_rise(void) {

    if (_state == State::LISTENING) {
        _timer.start();
        _timer.reset();

        _state = State::MEASURING;
    }
}

void HC_SR04::ping_fall(void) {
    timestamp_t elapsed_us;

    if (_state == State::MEASURING) {
        elapsed_us = _timer.read();
        _timer.stop();

        _latest_measurement_mm = (29 * elapsed_us) / 10;

        if (_measurement_callback) {
            _measurement_callback.call(_latest_measurement_mm);
        }

        _state = State::IDLE;
    }
}

void HC_SR04::trigger_ping(void) {
    _trigger = 1;
    wait_us(10);
    _trigger = 0;
    _state = State::LISTENING;

    if (_timeout_us > 0) {
        _timeout.attach(this, &HC_SR04::handle_timeout, _timeout_us);
    }
}