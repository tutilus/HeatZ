/*
  HeatZ.hpp - Pilot wire heating controller library for Arduino & Wiring
  Copyright (c) 2006 Tutilus.  All right reserved.
  @created_at: 26-11-2018
*/

#ifndef HEATZ_H
#define HEATZ_H

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Ticker.h>

#define DEFAULT_PERIOD 30  // Period of time for each shift (allow to break limits due of Int32)
#define ORDER_NB 4

typedef enum { HOURS, MINUTES, SECONDS } hzPeriod_t;
typedef enum { CONFORT, HORSGEL, ARRET, ECO } hzOrder_t; 
typedef enum { PIN1, PIN2 } hzPin_t;
class HeatZ
{
public:

    HeatZ();

    void begin(uint8_t, uint8_t, hzOrder_t, String);    
    void begin(uint8_t, uint8_t, hzOrder_t);
    void begin(uint8_t, uint8_t, String);
    void begin(uint8_t, uint8_t);

    // Event functions
    void onChange(void (*)(HeatZ*));
    
    // Scheduler task functions
    void toDefault(void); // Change heating to default mode

    bool isDefaultOrder(void);

    void changeOrder(hzOrder_t, int, hzPeriod_t);

    // Setters
    void setName(String);
    void setDefaultOrder(hzOrder_t);
    void setTimestamp(String);

    // Getters
    String getName();
    hzOrder_t getOrder();
    String getOrderLabel();
    hzOrder_t getDefaultOrder();
    String getDefaultOrderLabel();
    hzPeriod_t getPeriodUnit();
    String getTimestamp();
    int getDuration();
    hzPeriod_t getUnit();
    uint8_t getPinValue(hzPin_t);
    uint8_t getPinLabel(hzPin_t);

private:
    hzOrder_t order;
    hzOrder_t defaultOrder;
    uint8_t pins[2];
    uint8_t states[2];
    String name;  
    String timestamp;
    int duration;   // Time to wait before the end of the mode
    
    // Life cycle of the Heating
    hzPeriod_t periodUnit;
    int leftover;  // Time left in the mode when isn't default
    Ticker ticker; // timer to use (warning! limits to 77-min-ich max)

//    // Callback functions
    static void (*onchange_cb)(HeatZ*);
    static void internalCallback(HeatZ*);

    void wait(int);
    void wait(int, hzPeriod_t);
    bool finished(void);
    int timeLeftover(void);

    void setOrder(hzOrder_t);
    
    void setState(uint8_t, uint8_t);    
    String orderLabelFor(hzOrder_t);
    
};

#endif // HEATZ_H