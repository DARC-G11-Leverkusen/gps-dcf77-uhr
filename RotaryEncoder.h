/*******Interrupt-based Rotary Encoder reading Library*******
 * The code is written by Simon Merrett and bases on insight from Oleg Mazurov, Nick Gammon, rt and Steve Spence, and code from Nick Gammon.
 * It was put together to a library and was edited by Ralf Rumbler on 29.5.2018.
 */

#ifndef TestMenue_h
#define TestMenue_h

#include <Arduino.h>

// Rotary encoder declarations
static int pinA = 2;     	 // Our first hardware interrupt pin is digital pin 2
static int pinB = 3; 		 // Our second hardware interrupt pin is digital pin 3
volatile byte aFlag = 0; 	 // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
volatile byte bFlag = 0; 	 // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
int encoderPos = 0; 		 // this variable stores our current value of encoder position. Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
volatile byte oldEncPos = 0; // stores the last encoder position value so we can compare to the current reading and see if it has changed (so we know when to print to the serial monitor)
volatile byte reading = 0; 	 // somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent

// Button reading, including debounce without delay function declarations
byte _buttonPin;					   // variable that is needed to use the buttonPin variable (declared when running encoderBegin()) in this library
byte oldButtonState = HIGH;  		   // assume switch open because of pull-up resistor
const unsigned long debounceTime = 10; // milliseconds
unsigned long buttonPressTime;  	   // when the switch last changed state
boolean buttonPressed = 0; 			   // a flag variablet


void PinA(){ //Rotary encoder interrupt service routine for one encoder pin
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; // read all eight pin values then strip away all but pinA and pinB's values
  if(reading == B00001100 && aFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos --; //decrement the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00000100) bFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
  sei(); //restart interrupts
}
void PinB(){ //Rotary encoder interrupt service routine for the other encoder pin
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; //read all eight pin values then strip away all but pinA and pinB's values
  if (reading == B00001100 && bFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos ++; //increment the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00001000) aFlag = 1; //signal that we're expecting pinA to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

void encoderButtonRead(){
	// Button reading with non-delay() debounce - thank you Nick Gammon!
	byte buttonState = digitalRead (_buttonPin);
	if (buttonState != oldButtonState){
	    if (millis () - buttonPressTime >= debounceTime){ // debounce
	    	buttonPressTime = millis ();  	// when we closed the switch
	    	oldButtonState =  buttonState;  // remember for next time
	    if (buttonState == LOW){
	    	buttonPressed = 1;
	    }
	    else {
	    	buttonPressed = 0;
	    }
	  }  // end if debounce time up
	} // end of state change
}


void encoderBegin(byte buttonPin){	// buttonPin: this is the Arduino pin we are connecting the push button to
	_buttonPin = buttonPin;
	//Rotary encoder section of setup
	pinMode(pinA, INPUT_PULLUP); 	// set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
	pinMode(pinB, INPUT_PULLUP); 	// set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
	attachInterrupt(0,PinA,RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
	attachInterrupt(1,PinB,RISING); // set an interrupt on PinB, looking for a rising edge signal and executing the "PinB" Interrupt Service Routine (below)
	// button section of setup
	pinMode (buttonPin, INPUT_PULLUP); // setup the button pin
}

#endif











