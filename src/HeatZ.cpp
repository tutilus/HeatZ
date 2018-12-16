/*
  HeatZ.hpp - Pilot wire heating controller library for Arduino & Wiring
  Copyright (c) 2006 Tutilus.  All right reserved.

  This program is designed to work with a heating controller
  which is able to create specific wave from sinusoidal wave
  based on 2 pins declared in the constructor.
    - Pin1 will keep only positive part of the sinusoidal wave
    - Pin2 will keep only negative part.
  @created_at: 26-11-2018
*/

#include "HeatZ.h"

// Init static variables

void (*HeatZ::onchange_cb)(HeatZ*);

HeatZ::HeatZ() {}

void HeatZ::begin(uint8_t pin1, uint8_t pin2, hzOrder_t defaultOrder, String name) {
    pins[0] = pin1; // Negative current
    pins[1] = pin2; // Positive current
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    setName(name);
    setDefaultOrder(defaultOrder);
    setTimestamp("");
    toDefault();
}

void HeatZ::begin(uint8_t pin1, uint8_t pin2, hzOrder_t defaultOrder) { begin(pin1, pin2, defaultOrder, "room"); }

void HeatZ::begin(uint8_t pin1, uint8_t pin2, String name) { begin(pin1, pin2, HORSGEL, name); }

void HeatZ::begin(uint8_t pin1, uint8_t pin2) { begin(pin1, pin2, HORSGEL, "room"); }

void HeatZ::toDefault(void) {
    setOrder(this->defaultOrder);
    this->duration = 0;
}

void HeatZ::onChange(void (*function)(HeatZ*)) {
    onchange_cb = function;
}

void HeatZ::wait(int duration) {
    wait(duration, MINUTES);
}

/**
 * This function allow to wait more than the Ticker allow to do.
 * It splits the duration in number of period of time (DEFAULT_PERIOD) to 
 * extend the duration to more than one full day.
 * @param duration: Time in {periodType} to wait for the end
 *                  the current mode.
 */
void HeatZ::wait(int duration, hzPeriod_t periodUnit) {
    static int DefaultPeriod[] = { 1, 60, 300};
    int period; // period to wait for.

    // Save periodUnit for use into the callback function.
    this->periodUnit = periodUnit;
    
    // Define the period to wait depends on duration and period max
    // per round.
    if (duration == 0)
        period = 0; 
    else
        if (duration / DefaultPeriod[periodUnit] > 0)
            period = DefaultPeriod[periodUnit];
        else
            if (duration % DefaultPeriod[periodUnit] > 0)
                period = duration % DefaultPeriod[periodUnit];
            else
                period = DefaultPeriod[periodUnit];

    this->leftover = duration - period;
    switch (periodUnit) {
        case HOURS:
            period *= 3600; break;
        case MINUTES:
            period *= 60; break;
        case SECONDS:
            break;
    }
    // Launch a waiting period
    ticker.once(
        period, 
        HeatZ::internalCallback, 
        this
        );
}

void HeatZ::internalCallback(HeatZ* heat) {
    // End of waiting ?
    if (heat->finished()) {
        heat->toDefault();
    } else {
      // Keep going to wait on the left over duration
      heat->wait(heat->timeLeftover(), heat->getPeriodUnit());
    }
}

bool HeatZ::finished(void) {
    return (this->leftover == 0);
}

bool HeatZ::isDefaultOrder(void) {
    return (this->order == this->defaultOrder);
}

int HeatZ::timeLeftover(void) { return this->leftover; }

void HeatZ::changeOrder(hzOrder_t order, int duration, hzPeriod_t unit) {
    setOrder(order);
    // Save duration for the first call
    this->duration = duration;
    wait(duration, unit);
}

/**
 * Timestamp getter/setter
 * ESP8266 doesn't manage time. So, the Time (using NTP for example) have to be
 * declare outside this class, but this setter/getter allow to keep the data
 * inside the instance.
 */
void HeatZ::setTimestamp(String stamp) { this->timestamp = stamp; }

String HeatZ::getTimestamp() { return this->timestamp; }

int HeatZ::getDuration() { return this->duration; }

hzPeriod_t HeatZ::getUnit() { return this->periodUnit; }

void HeatZ::setOrder(hzOrder_t code) {
    static uint8_t WavePlus[] =  { LOW, HIGH, LOW, HIGH };
    static uint8_t WaveMinus[] = { LOW, LOW, HIGH, HIGH };
    
    if (code >= 0 && code < ORDER_NB) {
        this->order = code;
        setState(WavePlus[code], WaveMinus[code]);    
    }
    if (onchange_cb) { onchange_cb(this); }
 }

hzOrder_t HeatZ::getOrder() { return this->order; }

String HeatZ::getOrderLabel() { return orderLabelFor(this->order); }

void HeatZ::setName(String name) { this->name = name; }

String HeatZ::getName() { return this->name; }

void HeatZ::setDefaultOrder(hzOrder_t order) { this->defaultOrder = order; }

hzOrder_t HeatZ::getDefaultOrder() { return this->defaultOrder; }

String HeatZ::getDefaultOrderLabel() { return orderLabelFor(this->defaultOrder); }

hzPeriod_t HeatZ::getPeriodUnit() { return this->periodUnit; }

uint8_t HeatZ::getPinValue(hzPin_t pinNr) { 
    if (pinNr >= 0 && pinNr < 2) {
        return states[pinNr];
    } else {
        return -1;
    }
}

uint8_t HeatZ::getPinLabel(hzPin_t pinNr) { 
    if (pinNr >= 0 && pinNr < 2) {
        return pins[pinNr];
    } else {
        return -1;
    }
}

String HeatZ::orderLabelFor(hzOrder_t code) {
    static String Labels[] = {"Confort", "Hors gel", "Arrêt", "Eco" };
    return Labels[code];
}

void HeatZ::setState(uint8_t val1, uint8_t val2) {
    digitalWrite(pins[0], val1);
    states[0] = val1;
    digitalWrite(pins[1], val2);
    states[1] = val2;    
}
