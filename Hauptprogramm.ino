/*
 *	Dies ist das Hauptprogramm des Projektes "Arduino Uhr" der Jugendgruppe
 *	des OVs G11 Leverkusen von IGEL e.V. und DARC e.V. .
 *
 *	Teile des Programms für den Zugriff auf den EEPROM sind von Matthias Busse.
 */

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "RotaryEncoder.h"

LiquidCrystal lcd(4,5,9,10,11,12);


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
	lcd.begin(16, 2);
	encoderBegin(19);
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
	encoderButtonRead();
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









