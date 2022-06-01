/*  dataForController_t.h
 *
 *	  Alan Chatham - 2011
 *
 *  This is simply a typedef for a struct
 *   that holds information about controller
 *   button presses.  It is used by the controller
 *   libraries to pass information from an application
 *   to a library function that formats and sends
 *   appropriate controller data
 */

#ifndef DATA_FOR_CONTROLLER_T
#define DATA_FOR_CONTROLLER_T

#define BUTTON_ARRAY_SIZE 1 // 4
#define ANALOG_AXIS_ARRAY_SIZE 6 // 12

typedef struct dataForMegaController_t
{
	uint8_t buttonArray[BUTTON_ARRAY_SIZE];
	
	uint8_t dpad0LeftOn : 1;
	uint8_t dpad0UpOn : 1;
	uint8_t dpad0RightOn : 1;
	uint8_t dpad0DownOn : 1;
	
	uint8_t dpad1LeftOn : 1;
	uint8_t dpad1UpOn : 1;
	uint8_t dpad1RightOn : 1;
	uint8_t dpad1DownOn : 1;
	
	int16_t analogAxisArray[ANALOG_AXIS_ARRAY_SIZE];
} dataForMegaController_t;


#endif
