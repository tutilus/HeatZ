# heatz-arduino
Arduino library to control a pilot wire heatings using a ESP8266, for example, as the center of the controller.

# Main functions
It allows 4 orders: `CONFORT`, `HORSGEL`, `ARRET` and `ECO` (values for `hzOrder_t`).

The library is initialize with `begin()` function with one of the following:
```C++
// Set the controller
// @param pin1 is the digital pin to control the positive part of the sinusoidal
// @param pin2 is the digital pin to control the negative part of the sinusoidal
// @param defaultOrder (optional) is the order set to the control by default
// @param name (optional) is the label for this heatings
HeatZ::begin(uint8_t pin1, uint8_t pin2, hzOrder_t defaultOrder = HORSGEL, String name = "room")
```
To give an order to the controller, call the function `changeOrder` with the order and the duration (and the unit: `HOURS`, `MINUTES`, `SECONDS`.
Internal function use `Ticker` Arduino library to overide the behavior and allows to keep the duration longer (from 77min to more than 1 day).
```C++
// Set a new order for a duration (in a choosen unit)
// @param order is one a the four orders
// @param duration is the time to keep this order on
// @param unit is the unit for the duration
int changeOrder(hzOrder order, int duration, hzPeriod_t unit); 
```

```C++
onChange(void (*callback)(HeatZ* heat));
```

# Example

```C++
HeatZ heat;

heat.onChange(callback);
heat.begin(D1, D2);

heat.changeOrder(CONFORT, 25, MINUTES);
```
