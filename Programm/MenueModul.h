/*
 *	Menue Modul des Projektes "Arduino Uhr" der Jugendgruppe
 *	des OVs G11 Leverkusen von IGEL e.V. und DARC e.V. .
 *
 * 	geschrieben von Ralf Rumbler 29.5.2018
 */

#ifndef MenueModul_h
#define MenueModul_h

#include <Arduino.h>
#include "RotaryEncoder.h"

LiquidCrystal lcd(4,5,9,10,11,12);

//MenueFuehrung
byte menueEintragSprung = LOW;
byte menueFuehrungZustand = HIGH;
byte menueRotaryEncoder = 1;
byte menueZurueckPfeil = 1;
long menueAdresse = 1;
int menueEintraegeAnzahl;
char* menueCursorZeichen[1];
char* menueEintrag[8];
int menueAktion[8];
//MenueFuehrung (innerhalb MenueFuehrung())
byte stateAktion = LOW;
byte CursorPos = 0;  // 0 = oben ; 1 = unten
int Auswahl = 1; 	 // Auswahl sagt auf welchem MenueEintrag[] Cursor ist

void menueClear(){
	Auswahl = 1;
	CursorPos = 0;
	encoderPos = 0;
	lcd.clear();
	memset(menueEintrag, 0, sizeof menueEintrag);
	memset(menueCursorZeichen, 0, sizeof menueCursorZeichen);
}

void menueEbeneZurueck(){
	int multiplier = 10;

	while(1){
		if(menueAdresse < multiplier*10){
			while(menueAdresse >= multiplier){
				menueAdresse -= multiplier;
			}
			break;
		}
		multiplier *= 10;
	}

	menueClear();
}

void menueEbeneVor(){
	int multiplier = 10;

	while(1){
		if(menueAdresse < multiplier){
			menueAdresse += (multiplier*Auswahl);
			break;
		}
		else{
			multiplier *= 10;
		}
	}

	menueClear();
}

void menueFuehrung(){
	if(menueFuehrungZustand == HIGH){
		if(menueRotaryEncoder == 1 && menueEintraegeAnzahl+menueZurueckPfeil > 1){
			if(encoderPos >= 1 && CursorPos == 0){
				Auswahl++;
				CursorPos = 1;
				lcd.clear();
			}
			else if(encoderPos >= 1 && CursorPos == 1){
				Auswahl++;
				//CursorPos bleibt 1

				if(Auswahl > menueEintraegeAnzahl+menueZurueckPfeil){  //Cursor ist schon ganz unten
					if(menueEintragSprung == HIGH){
						Auswahl = 1;
						CursorPos = 0;
						lcd.clear();
					}
					else{
						Auswahl = menueEintraegeAnzahl+menueZurueckPfeil;
					}
				}
				else lcd.clear();
			}
			else if(encoderPos <= -1 && CursorPos == 0){
				Auswahl--;
				//CursorPos bleibt 0

				if(Auswahl < 1){  // Cursor ist schon ganz oben
					if(menueEintragSprung == HIGH){
						Auswahl = menueEintraegeAnzahl+menueZurueckPfeil;
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
			lcd.print(menueEintrag[Auswahl]);

			if(menueEintraegeAnzahl+menueZurueckPfeil > 1){
				lcd.setCursor(0,1);
				lcd.print(menueEintrag[Auswahl+1]);
			}
		}
		else if(CursorPos == 1){
			lcd.setCursor(0,0);
			lcd.print(menueEintrag[Auswahl-1]);
			if(menueEintraegeAnzahl+menueZurueckPfeil > 1){
				lcd.setCursor(0,1);
				lcd.print(menueEintrag[Auswahl]);
			}
		}

		lcd.setCursor(15, CursorPos);
		lcd.print(menueCursorZeichen[0]);

		//Serial.print(encoderPos);
		//Serial.print("  ");
		//Serial.println(Auswahl);


		/////////////////////////////

		if(menueZurueckPfeil == 1){
			menueEintrag[menueEintraegeAnzahl+1] = "<==";
			menueAktion[menueEintraegeAnzahl+1] = -1;
		}

		//////////////////////////////////////////////////////////////////////////

		encoderButtonRead();
		if(encoderButtonPressed == LOW){
			stateAktion = HIGH;		//eine Aktion kann durch drücken des Knopfes durchgeführt werden
		}
		if(encoderButtonPressed == HIGH && stateAktion == HIGH){
			if(menueAktion[Auswahl] == -1){		//Auswahl ist: in nächst höhere Ebene/Adresse
				//zurückkehren in übergeordnete Ebene
				menueEbeneZurueck();
			}
			else if(menueAktion[Auswahl] == 1){	//Auswahl ist: in nächst tiefere Ebene/Adresse
				//tiefergehen in tiefere Ebene
				menueEbeneVor();
			}
			stateAktion = LOW;
		}
	}
}

void menueBegin(){
	lcd.begin(16, 2);
}

#endif







