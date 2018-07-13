/*******Interrupt-based Rotary Encoder reading Library*******
 * The code is written by Simon Merrett and bases on insight from Oleg Mazurov,
 * Nick Gammon, rt and Steve Spence, and code from Nick Gammon.
 *
 * It was put together to a library and was edited by Ralf Rumbler, DO3KV
 * on 13.7.2018.
 */

/********ERKLÄRUNG DER FUNKTIONEN & VARIABLEN DER LIBRARY********
 *
 * 	Funktionen:
 * 	==============================================================================
 *		encoderBegin(pinA, pinB, buttonPin)
 *			Diese Funktion muss in der Setup() Funktion aufgerufen werden, damit
 *			die Library funktionsfähig ist. Durch sie wird auch das Lesen des
 *			Rotary Encoders per Interrupts aktiviert.
 *
 *			Zu deklarieren sind...
 *			1. der erste Pin "pinA" und der zweite Pin "pinB", an dem der Rotary
 *			   Encoder angeschlossen ist.
 *			2. der Pin "buttonPin" mit dem der Zustand des Buttons (per im
 *			   Mikrocontroller eingebautem Pull-Up Widerstand =>
 *			   pinMode(buttonPin, INPUT_PULLUP)) gelesen wird.
 *
 *		encoderButtonRead()
 *			Diese Funktion liest den Zustand des Buttons ab und speichert ihn
 *			in der Variable "encoderButtonPressed" ab.
 *
 *
 * 	Ausgabe-Variablen:
 * 	==============================================================================
 * 		encoderPos
 * 			Diese Variable Beinhaltet die Position des Rotary Encoders. Nach
 * 			Starten des Programm ist sie als erstes 0.
 * 			Bei Drehen nach rechts wird die aktuelle Position + 1 gerechnet.
 * 			Bei Drehen nach links wird die aktuelle Position - 1 gerechnet.
 * 			Die Variable kann verändert werden.
 *
 * 		encoderButtonPressed
 * 			Diese Variable Beinhaltet den Zustand des Buttons des Rotary Encoders.
 * 			Wenn die Variable gleich LOW ist, ist der Button nicht gedrückt.
 * 			Wenn die Variable gleich HIGH ist, ist der Button gedrückt.
 */

//!!!!!Erklärung zu Z.56 fehlt!!!!!

#ifndef RotaryEncoder_h
#define RotaryEncoder_h

#include <Arduino.h>

// Rotary encoder declarations
volatile byte aFlag = 0; 	 		// let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
volatile byte bFlag = 0; 			// let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
int encoderPos = 0; 		 		// this variable stores our current value of encoder position. Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
volatile byte encoderChanged = 0;
volatile byte reading = 0; 	 		// somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent

// Button reading, including debounce without delay function declarations
byte _buttonPin;					   // variable that is needed to use the buttonPin variable (declared when running encoderBegin()) in this library
byte oldButtonState = HIGH;  		   // assume switch open because of pull-up resistor
const unsigned long debounceTime = 10; // milliseconds
unsigned long buttonPressTime;  	   // when the switch last changed state
boolean encoderButtonPressed = 0;	   // a flag variablet


void PinA(){ //Rotary encoder interrupt service routine for one encoder pin
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; // read all eight pin values then strip away all but pinA and pinB's values
  if(reading == B00001100 && aFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos --; //decrement the encoder's position count
    encoderChanged = HIGH;
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
    encoderChanged = HIGH;
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
	    	encoderButtonPressed = 1;
	    }
	    else {
	    	encoderButtonPressed = 0;
	    }
	  }  // end if debounce time up
	} // end of state change
}


void encoderBegin(int pinA, int pinB, byte buttonPin){	// buttonPin: this is the Arduino pin we are connecting the push button to
	_buttonPin = buttonPin;
	static int _pinA = pinA;   	 // Our first hardware interrupt pin is digital pin 2
	static int _pinB = pinB;	 // Our second hardware interrupt pin is digital pin 3

	//Rotary encoder section of setup
	pinMode(_pinA, INPUT_PULLUP); 	// set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
	pinMode(_pinB, INPUT_PULLUP); 	// set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
	attachInterrupt(digitalPinToInterrupt(_pinA),PinA,RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
	attachInterrupt(digitalPinToInterrupt(_pinB),PinB,RISING); // set an interrupt on PinB, looking for a rising edge signal and executing the "PinB" Interrupt Service Routine (below)
	// button section of setup
	pinMode(buttonPin, INPUT_PULLUP); // setup the button pin
}

#endif











