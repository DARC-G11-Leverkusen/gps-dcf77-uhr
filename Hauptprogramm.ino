/*
 *	Menü-Programmmodul Ver. 2.004, des Projektes "Arduino Uhr" der Jugendgruppe
 *	des OVs G11 Leverkusen von IGEL e.V. und DARC e.V.
 *
 *	geschrieben von Ralf Rumbler, DO3KV, 04.04.2018
 *
 *	Teile des Programms für den Zugriff auf den EEPROM sind von Matthias Busse
 *	und Teile für die Benutzung des Rotary Encoders und dessen Button sind von Simon Merrett
 */

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <TestMenue.h>

LiquidCrystal lcd(4,5,9,10,11,12);


// Rotary encoder declarations
static int pinA = 2;     	 // Our first hardware interrupt pin is digital pin 2
static int pinB = 3; 		 // Our second hardware interrupt pin is digital pin 3
volatile byte aFlag = 0; 	 // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
volatile byte bFlag = 0; 	 // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
int encoderPos = 0; 		 // this variable stores our current value of encoder position. Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
volatile byte oldEncPos = 0; // stores the last encoder position value so we can compare to the current reading and see if it has changed (so we know when to print to the serial monitor)
volatile byte reading = 0; 	 // somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent

// Button reading, including debounce without delay function declarations
const byte buttonPin = 19; 			   // this is the Arduino pin we are connecting the push button to
byte oldButtonState = HIGH;  		   // assume switch open because of pull-up resistor
const unsigned long debounceTime = 10; // milliseconds
unsigned long buttonPressTime;  	   // when the switch last changed state
boolean buttonPressed = 0; 			   // a flag variablet

//MenueFuehrung
byte EintragSprung = LOW;
byte MenueFuehrungState = HIGH;
byte RotaryEncoderState = 1;
long Adresse = 1;
int MenueEintraegeAnzahl;
char* CursorZeichen[1];
char* MenueEintrag[8];
int Aktion[8];
//MenueFuehrung (innerhalb MenueFuehrung())
byte stateAktion = LOW;
byte CursorPos = 0;  // 0 = oben ; 1 = unten
int Auswahl = 1; 	 // Auswahl sagt auf welchem MenueEintrag[] Cursor ist

///////////////////////////////////////////////////////////////////////////////////

void setup(){
	//Rotary encoder section of setup
	pinMode(pinA, INPUT_PULLUP); 	// set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
	pinMode(pinB, INPUT_PULLUP); 	// set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
	attachInterrupt(0,PinA,RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
	attachInterrupt(1,PinB,RISING); // set an interrupt on PinB, looking for a rising edge signal and executing the "PinB" Interrupt Service Routine (below)
	// button section of setup
	pinMode (buttonPin, INPUT_PULLUP); // setup the button pin
	// DEBUGGING section of setup

	pinMode(13, OUTPUT);

	lcd.begin(16, 2);
}

///////////////////////////////////////////////////////////////////////////////////

void loop(){
	if(MenueFuehrungState == HIGH) MenueFuehrungA();

	switch(Adresse){
		//Adresse max. =2147483647
		case   1:{
			MenueFuehrungState = HIGH;	//	Menü an/aus (HIGH = an)
			 RotaryEncoderState = 1;	/*  1: Rotary Encoder wird von MenueFuehrung() benutzt (standart)
			 	 	 	 	 	 	 	 *  0: Rotary Encoder wird nicht von MenueFuehrung() benutzt (sondern z.B. für Auswahl von Infos)
			 	 	 	 	 	 	 	 */
			 EintragSprung = LOW;	   	/*	Sprung bei Ende des Menüs zum ersten Eintrag, wenn heruntergedreht wird
			 						   	 *	UND Sprung bei Anfang des Menüs zum letzten Eintrag, wenn heraufgedreht wird:
			 							 *	an/aus (HIGH = an)
			 							 */
			 CursorZeichen[0] = "<";	//	Cursorzeichen (bei Auswahl von Infos z.B. "X")
			 MenueEintraegeAnzahl = 3;  //	Anzahl der Menüeinträge

			 MenueEintrag[1] = "AA"; 	//  Inhalt vom Menüeintrag
			 Aktion[1] = 0;				/*	Aktion bei drücken Auf Menüeintrag ist...
			 	 	 	 	 	 	 	 *	-1 = in nächst höhere Ebene/Adresse gehen
			 	 	 	 	 	 	 	 *	  => !! ein "Zurück-Pfeil" ist bei MenueFuehrung() schon implementiert !!
			 	 	 	 	 	 	 	 *	 0 = (noch) keine Aktion
			 	 	 	 	 	 	 	 *	 1 = in nächst tiefere Ebene/Adresse gehen
			 	 	 	 	 	 	 	 */
			 MenueEintrag[2] = "BBB";
			 Aktion[2] = 1;
			 MenueEintrag[3] = "C";
			 Aktion[3] = 1;
		}break;

		case   21:{
			MenueFuehrungState = HIGH;
			 EintragSprung = HIGH;
			 MenueEintraegeAnzahl = 4;

			 MenueEintrag[1] = "Q1";
			 Aktion[1] = 0;
			 MenueEintrag[2] = "ZPO";
			 Aktion[2] = 0;
			 MenueEintrag[3] = "6";
			 Aktion[3] = 0;
			 MenueEintrag[4] = "90";
			 Aktion[4] = 1;

			 Blink();
		}break;

		default:{
			MenueFuehrungState = HIGH;
			 MenueEintraegeAnzahl = 1;

			 MenueEintrag[1] = "*Ebene leer*";
			 Aktion[1] = 0;
		}break;
	}

	if(MenueFuehrungState == HIGH) MenueFuehrungB();
}

///////////////////////////////////////////////////////////////////////////////////

void MenueFuehrungA(){
	ButtonRead();
	if(buttonPressed == LOW){
		stateAktion = HIGH;		//eine Aktion kann durch drücken des Knopfes durchgeführt werden
	}
	if(buttonPressed == HIGH && stateAktion == HIGH){
		int multiplier = 10;

		if(Aktion[Auswahl] == -1){		//Auswahl ist: in nächst höhere Ebene/Adresse
			//zurückkehren in übergeordnete Ebene
			while(1){
				if(Adresse < multiplier*10){
					while(Adresse >= multiplier){
						Adresse -= multiplier;
					}
					break;
				}
				multiplier *= 10;
			}

			Auswahl = 1;
			CursorPos = 0;
		}
		else if(Aktion[Auswahl] == 1){	//Auswahl ist: in nächst tiefere Ebene/Adresse
			while(1){
				if(Adresse < multiplier){
					Adresse += (multiplier*Auswahl);
					break;
				}
				else{
					multiplier *= 10;
				}
			}

			Auswahl = 1;
			CursorPos = 0;
		}

		lcd.clear();
		stateAktion = LOW;
	}
}
void MenueFuehrungB(){
	if(RotaryEncoderState == 1){
		if(encoderPos >= 1 && CursorPos == 0){
			Auswahl++;
			CursorPos = 1;
			lcd.clear();
		}
		else if(encoderPos >= 1 && CursorPos == 1){
			Auswahl++;
			//CursorPos bleibt 1

			if(Auswahl > MenueEintraegeAnzahl+1){  //Cursor ist schon ganz unten
				if(EintragSprung == HIGH){
					Auswahl = 1;
					CursorPos = 0;
					lcd.clear();
				}
				else{
					Auswahl = MenueEintraegeAnzahl+1;
				}
			}
			else lcd.clear();
		}
		else if(encoderPos <= -1 && CursorPos == 0){
			Auswahl--;
			//CursorPos bleibt 0

			if(Auswahl < 1){  // Cursor ist schon ganz oben
				if(EintragSprung == HIGH){
					Auswahl = MenueEintraegeAnzahl+1;
					CursorPos = 1;
					lcd.clear();
				}
				else{
					Auswahl = 1;
				}
			}
			else lcd.clear();
		}
		else if(encoderPos <= -1 && CursorPos == 1){
			Auswahl--;
			CursorPos = 0;
			lcd.clear();
		}
		encoderPos = 0;
	}

	/////////////////////////////

	if(CursorPos == 0){
		lcd.setCursor(0,0);
		lcd.print(MenueEintrag[Auswahl]);
		lcd.setCursor(0,1);
		lcd.print(MenueEintrag[Auswahl+1]);
	}
	else if(CursorPos == 1){
		lcd.setCursor(0,0);
		lcd.print(MenueEintrag[Auswahl-1]);
		lcd.setCursor(0,1);
		lcd.print(MenueEintrag[Auswahl]);
	}

	lcd.setCursor(15, CursorPos);
	lcd.print(CursorZeichen[0]);

	//Serial.print(encoderPos);
	//Serial.print("  ");
	//Serial.println(Auswahl);


	/////////////////////////////

	MenueEintrag[MenueEintraegeAnzahl+1] = "<==";
	Aktion[MenueEintraegeAnzahl+1] = -1;
}


void ButtonRead(){
	// Button reading with non-delay() debounce - thank you Nick Gammon!
	byte buttonState = digitalRead (buttonPin);
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


int eepromReadInt(int adr) {
  // 2 Byte Integer Zahl aus dem EEPROM lesen an der Adresse
  // Eingabe:
  //   adr: Adresse +0 und +1 wird gelesen
  // Ausgabe: int Wert
  //
  // Matthias Busse 5.2014 V 1.0

  byte low, high;

  low=EEPROM.read(adr);
  high=EEPROM.read(adr+1);
  return low + ((high << 8)&0xFF00);
}
void eepromWriteInt(int adr, int wert) {
  // 2 Byte Integer Zahl im EEPROM ablegen an der Adresse
  // Eingabe:
  //   adr: Adresse +0 und +1 wird geschrieben
  //   wert: möglicher Wertebereich -32,768 bis 32,767
  // Ausgabe:
  //   -
  // 2 Byte Platz werden belegt.
  //
  // Matthias Busse 5.2014 V 1.0
  //
  // (Speicher von Arduino UNO (ATmega328P) = 1024 Bytes)

  byte low, high;

  low=wert&0xFF;
  high=(wert>>8)&0xFF;
  EEPROM.write(adr, low); // dauert 3,3ms
  EEPROM.write(adr+1, high);
  return;
}









