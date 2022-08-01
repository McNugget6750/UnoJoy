/*
* Modified version by Timo Birnschein (timo.birnschein@googlemail.com) 2022/06/01 
* I need less buttons and less axis but at least 10 bit axis resolution. 16 bit would be better.
*/

#include "MegaJoy.h"

#define FLAPS_MIN 59
#define FLAPS_MAX 244
#define FLAPS_RANGE (FLAPS_MAX - FLAPS_MIN)

uint16_t a0_filtered = 0;
uint16_t a1_filtered = 0;
uint16_t a2_filtered = 0;
uint16_t a3_filtered = 0;
uint16_t a4_filtered = 0;
uint16_t a4_scaled = 0;
uint16_t a5_filtered = 0;

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
  controllerData.analogAxisArray[0] = analogRead(A0); // Throttle
//  Serial.println(controllerData.analogAxisArray[0]);
  controllerData.analogAxisArray[1] = analogRead(A1); // Trim
//  Serial.println(controllerData.analogAxisArray[1]);
  controllerData.analogAxisArray[2] = analogRead(A2); // Propeller
//  Serial.println(controllerData.analogAxisArray[2]);
  controllerData.analogAxisArray[3] = analogRead(A3); // Mixture
//  Serial.println(controllerData.analogAxisArray[3]);

  
  int16_t flapsScaler = analogRead(A4); // 56 - 245 must be scaled up to 0 to 1024 with filter
//  Serial.println(flapsScaler);

  if (flapsScaler < FLAPS_MIN) flapsScaler = FLAPS_MIN;
  if (flapsScaler > FLAPS_MAX) flapsScaler = FLAPS_MAX;
  flapsScaler = flapsScaler - FLAPS_MIN;
//  controllerData.analogAxisArray[4] = (uint16_t)((1023.0f / 189.0f) * (float)flapsScaler); // 68 - 255 must be scaled up to 0 to 1024 with filter -- KNOWN WORKING
//  controllerData.analogAxisArray[4] = (uint16_t)((1023.0f / (float)FLAPS_RANGE) * (float)flapsScaler); // 68 - 255 must be scaled up to 0 to 1024 with filter
  
  a4_scaled = (uint16_t)((1023.0f / (float)FLAPS_RANGE) * (float)flapsScaler); // 68 - 255 must be scaled up to 0 to 1024 with filter
  a4_scaled = a4_scaled << 6; // convert 10 bit value into 16 bit value for filtering
  a4_filtered = (uint16_t)(a4_filtered * 0.99f);
  a4_filtered += a4_scaled * 0.01f;
  controllerData.analogAxisArray[4] = (uint16_t)((a4_filtered >> 6) & 0x03FF);

//  Serial.println(controllerData.analogAxisArray[4]);
  

  int16_t tankSelectorScaler = analogRead(A5); // 56 - 245 must be scaled up to 0 to 1024 with filter
  tankSelectorScaler = tankSelectorScaler - 142;
  if (tankSelectorScaler < 0) tankSelectorScaler = 0;
  if (tankSelectorScaler > 737) tankSelectorScaler = 737;
  controllerData.analogAxisArray[5] = (uint16_t)((1023.0f / 737.0f) * (float)tankSelectorScaler); // 142 - 878 must be scaled up to 0 to 1024 with filter
  controllerData.dpad0RightOn = (controllerData.analogAxisArray[5] >= 0 && controllerData.analogAxisArray[5] < 341) ? 1 : 0;
  controllerData.dpad0UpOn = (controllerData.analogAxisArray[5] >= 341 && controllerData.analogAxisArray[5] < 682) ? 1 : 0;
  controllerData.dpad0LeftOn = (controllerData.analogAxisArray[5] >= 682 && controllerData.analogAxisArray[5] <= 1023) ? 1 : 0;
//  Serial.println(controllerData.analogAxisArray[5]);
//  Serial.println("\r\n");
  
//  controllerData.analogAxisArray[6] = analogRead(A6);  // Reduce number of analog axis to just 6 - we only need six
//  controllerData.analogAxisArray[7] = analogRead(A7);
//  controllerData.analogAxisArray[8] = analogRead(A8);
//  controllerData.analogAxisArray[9] = analogRead(A9);
//  controllerData.analogAxisArray[10] = analogRead(A10);
//  controllerData.analogAxisArray[11] = analogRead(A11);
  
  // And return the data!
  return controllerData;
}

void setupPins(void){
  // Set all the digital pins as inputs
  // with the pull-up enabled, except for the 
  // two serial line pins
  for (int i = 2; i < ((BUTTON_ARRAY_SIZE*8) + 2); i++){
    pinMode(i, INPUT);
    digitalWrite(i, HIGH);
  }
}

void setup(){
  setupPins();
  setupMegaJoy();
}

void loop(){
  // Always be getting fresh data
  megaJoyControllerData_t controllerData_buffer_1 = getControllerData();
//  megaJoyControllerData_t controllerData_buffer_1 = getBlankDataForMegaController();
  setControllerData(controllerData_buffer_1);
}
