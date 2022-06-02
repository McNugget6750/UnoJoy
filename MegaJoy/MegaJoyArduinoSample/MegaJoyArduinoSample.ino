/*
* Modified version by Timo Birnschein (timo.birnschein@googlemail.com) 2022/06/01 
* I need less buttons and less axis but at least 10 bit axis resolution. 16 bit would be better.
*/

#include "MegaJoy.h"

void setup(){
  setupPins();
  setupMegaJoy();
}

void loop(){
  // Always be getting fresh data
  megaJoyControllerData_t controllerData = getControllerData();
  setControllerData(controllerData);
}

void setupPins(void){
  // Set all the digital pins as inputs
  // with the pull-up enabled, except for the 
  // two serial line pins
//  for (int i = 2; i < 54; i++){
  for (int i = 2; i < ((BUTTON_ARRAY_SIZE*8) + 2); i++){
    pinMode(i, INPUT);
    digitalWrite(i, HIGH);
  }
}

megaJoyControllerData_t getControllerData(void){
  
  // Set up a place for our controller data
  //  Use the getBlankDataForController() function, since
  //  just declaring a fresh dataForController_t tends
  //  to get you one filled with junk from other, random
  //  values that were in those memory locations before
  megaJoyControllerData_t controllerData = getBlankDataForMegaController();
  // Since our buttons are all held high and
  //  pulled low when pressed, we use the "!"
  //  operator to invert the readings from the pins
//  for (int i = 2; i < 54; i++){
  for (int i = 2; i < ((BUTTON_ARRAY_SIZE*8) + 2); i++){
    controllerData.buttonArray[(i - 2) / 8] |= (!digitalRead(i)) << ((i - 2) % 8);
  }
  
  // Set the analog sticks
  //  Since analogRead(pin) returns a 10 bit value,
  //  we need to perform a bit shift operation to
  //  lose the 2 least significant bits and get an
  //  8 bit number that we can use 
  controllerData.analogAxisArray[0] = analogRead(A0);
  controllerData.analogAxisArray[1] = analogRead(A1);
  controllerData.analogAxisArray[2] = analogRead(A2);
  controllerData.analogAxisArray[3] = analogRead(A3);
  controllerData.analogAxisArray[4] = (uint16_t)((1023.0f / 190.0f) * ((float)analogRead(A4) - 68.0f)); // 68 - 255 must be scaled up to 0 to 1024 with filter
  controllerData.analogAxisArray[5] = (uint16_t)((1023.0f / 740.0f) * ((float)analogRead(A5) - 140.0f)); // 142 - 878 must be scaled up to 0 to 1024 with filter
//  controllerData.analogAxisArray[6] = analogRead(A6);  // Reduce number of analog axis to just 6 - we only need six
//  controllerData.analogAxisArray[7] = analogRead(A7);
//  controllerData.analogAxisArray[8] = analogRead(A8);
//  controllerData.analogAxisArray[9] = analogRead(A9);
//  controllerData.analogAxisArray[10] = analogRead(A10);
//  controllerData.analogAxisArray[11] = analogRead(A11);

//  Serial.println(controllerData.analogAxisArray[4]);
  
  // And return the data!
  return controllerData;
}
