# HeatZ

Arduino library to control a pilot wire heatings using a ESP8266 board as the center of the controller.

## Features

* drive PW heatings with 4 orders 
* set default order
* change order for a period of time defined in seconds, minutes or hours.
* manage events

## Quickstart

Define heating with pin D1 and D2 to manage sinusoidal (so order PW heating).
Swap order to CONFORT position for 25 minutes.

```C++
HeatZ heat;

void setup() {
    heat.begin(D1, D2, ARRET);

    heat.changeOrder(CONFORT, 25, MINUTES);

}

void loop() { // unused }
```

This following example shows how to use events as `onChange`. It prints the label order for as long as the order changes. 

> It is called for the first time the heating is switch to default order.

```C++
HeatZ heat;

void callback(HeatZ* h) {
    Serial.print("Heating in mode ");
    Serial.println(h->getOrderLabel());
}

void setup() {
    heat.onChange(callback);
    heat.begin(D1, D2, ARRET);
    heat.changeOrder(CONFORT, 25, MINUTES);
}

void loop() { // unused }
```

## API

### Constants

* Order (type `hzOrder_t`) values: `CONFORT`, `HORSGEL`, `ARRET` and `ECO`
* Unit of time (type `hzPeriod_t`) values: `HOURS`, `MINUTES`, `SECONDS`
* One of the pins used by the heating (type `hzPin_t`): `PIN1` or `PIN2`

### Functions

The library is initialize with `begin()` function with one of the following:
```C++
HeatZ::begin(uint8_t pin1, uint8_t pin2, hzOrder_t defaultOrder = HORSGEL, String name = "room")
```
Parameters:
* `pin1` is the digital pin to control the positive part of the sinusoidal
* `pin2` is the digital pin to control the negative part of the sinusoidal
* `defaultOrder` (`optional`) is the order set to the control by default
* `name` (`optional`) is the label for this heatings

And swap to a different order given by `changeOrder` function for a duration.
> This function uses `Ticker` Arduino library. 

```C++
int changeOrder(hzOrder order, int duration, hzPeriod_t unit); 
```
Parameters:
* `order` to swap to
* `duration` to keep this order on
* `unit` for the duration

### Events

```C++
onChange(void (*callback)(HeatZ* heat));
```
