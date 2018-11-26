#include <HeatZ.h>

// Create an order for heating
HeatZ heat;

void callback(HeatZ* h) {
  Serial.println("**** New order ****");
  Serial.print("order: ");
  // Order code
  Serial.print(h->getOrder());

  // Point the fact is a default order
  if (h->isDefaultOrder()) { 
    Serial.print(" (default) ");
  }
  
  // Get label for the new order
  Serial.print(" | label: ");
  Serial.println(h->getOrderLabel());
  
  // Get data on the pin 1
  Serial.print( "[ PIN1: ");
  Serial.print(h->getPinLabel(PIN1));
  Serial.print(" => ");
  Serial.print(h->getPinValue(PIN1)); 
  
  // Get data on the pin 2
  Serial.print( "| PIN1: ");
  Serial.print(h->getPinLabel(PIN2));
  Serial.print(" => ");
  Serial.print(h->getPinValue(PIN2));
  Serial.println(" ]");
}

void setup() {
  Serial.begin(115200);  

  heat.onChange(callback);
  heat.begin(D1, D2);
  
  Serial.println("***** Heating *****"); 
  Serial.print("Name: ");
  Serial.println(heat.getName());
  
  // Change order
  heat.changeOrder(CONFORT, 30, SECONDS);
}

// Not used.
void loop() { }