/*
 *	Menü-Modul des Projektes "Arduino Uhr" der Jugendgruppe
 *	des OVs G11 Leverkusen von IGEL e.V. und DARC e.V. .
 *
 * 	geschrieben von Ralf Rumbler, DO3KV 25.10.2018
 */

/********ERKLÄRUNG DER FUNKTIONEN & VARIABLEN DES MODULS********
 *
 * 		Funktionen:
 * 		==========================================================================
 * 			menueBegin()
 * 				Diese Funktion muss in der Setup() Funktion aufgerufen werden,
 * 				damit das Modul funktionsfähig ist.
 *
 * 			menueFuehrung()
 * 				Diese Funktion muss am Ende der Loop() Funktion abgerufen werden,
 * 				damit Menüs geführt werden können.
 *
 * 			menueReset()
 * 				Durch abrufen dieser Funktion werden alle Deklarationen/Zuweisungen
 * 				des aktuellen Menüs zurückgesetzt.
 *
 * 			menueEbeneTiefer()
 * 				Durch abrufen dieser Funktion wird zur nächst tieferen Ebene
 * 				gesprungen (ohne menueReset() auszuführen), indem die menueAdresse
 * 				verändert wird (siehe menueAdresse).
 *
 * 			menueEbeneHoeher()
 * 				Durch abrufen dieser Funktion wird zur nächst höheren Ebene
 * 				gesprungen (ohne menueReset() auszuführen), indem die menueAdresse
 * 				verändert wird (siehe menueAdresse).
 *
 *
 *		ausgebende/Werte tragende  Variablen:
 * 		==========================================================================
 *			menueAdresse
 *				Diese Variable trägt die Adresse der aktuellen Ebene. Nach Starten
 *				des Programm ist sie als erstes 1.
 *
 *				springen in nächst tiefere Ebene:
 *				----------------------------------
 *				Wenn in die nächst tiefere Ebene gesprungen wird, wird  "[aktuelle
 *				Adresse] + 10^[Anzahl der Stellen der aktuellen Adresse]
 *				*menueAuswahl" gerechnet.
 *				Bsp.: Wenn die menueAdresse zuerst 1 ist, der 4. Menüpunkt
 *				ausgewählt ist (menueAuswahl = 4) und in die nächst tiefere Ebene
 *				gesprungen wird, ist die Adresse danach 41. Wenn danach der 3.
 *				Menüpunkt ausgewählt ist und in die nächst tiefere Ebene gesprungen
 *				wird, ist die Adresse danach 341 usw. .
 *
 *				springen in nächst höhere Ebene:
 *				----------------------------------
 *				Wenn in die nächst höhere Ebene gesprungen wird, wird die erste
 *				Stelle der Adresse der aktuellen Ebene abgezogen.
 *				Bsp.: Wenn menueAdresse zuerst 267 ist und in die nächst höhere
 *				Ebene gesprungen wird, ist die Adresse danach 67.
 *
 *			menueAuswahl
 *				Diese Variable entspricht der Nummer des aktuell ausgewählten
 *				Menü-Eintrages.
 *
 *			menueCursorPos
 *				Diese Variable beinhaltet die Zeile, in der sich der Cursor
 *				befindet (0 = Zeile 1, 1 = Zeile 2).
 *
 *
 * 		einzustellende Variablen:
 * 		==========================================================================
 * 			menueFuehrungZustand = HIGH/LOW;
 * 				Diese Deklaration aktiviert (HIGH), oder deaktiviert (LOW) alle
 * 				Funktionen des Menü-Moduls.
 *
 * 			menueEintragSprung = HIGH/LOW;
 * 				Diese Deklaration aktiviert (HIGH), oder deaktiviert (LOW)...
 * 				1. das Springen zum ersten Eintrag, bei weiterdrehen des Rotary
 * 				Encoders, wenn der letzte Menü-Eintrag erreicht wurde.
 * 				2. das Springen zum letzten Eintrag bei, weiterdrehen des Rotary
 * 				Encoders, wenn der erste Menü-Eintrag erreicht wurde.
 *
 * 			menueRotaryEncoder = HIGH/LOW;
 * 				Diese Deklaration aktiviert (HIGH), oder deaktiviert (LOW), die
 * 				Benutzung vom Rotary Encoder vom Menü-Modul.
 *
 * 			menueZurueckPfeil = HIGH/LOW;
 * 				Diese Deklaration aktiviert (HIGH), oder deaktiviert (LOW), das
 * 				Hinzufügen von einem Pfeil unter den aktuellen Menü-Einträgen,
 * 				mit welchen man durch Klicken zur nächst übergeordneten Ebene
 * 				gelangen kann.
 *
 * 			menueEinträgeAnzahl = X;
 * 				Durch die Deklaration dieser Variable wird die Anzahl der
 * 				Menü-Einträge X (min. 0, max. 8 einschließlich Zurück-Pfeil Eintrag)
 * 				der Menü-Ebene festgelegt.
 *
 * 			menueCursorZeichen = X;
 * 				Diesem String wird das Zeichen X, welches als Cursor benutzt werden
 * 				soll, zugewiesen (wie z.B. "<").
 *
 *
 *
 * 			menueEintrag[Y][Z] = X;
 * 				Diesem String wird die Zeichenfolge X (max. 14 Zeichen) zugewiesen,
 * 				welche der Menü-Eintrag Z (min. 1, max.5) in Zeile Y (min. 1, max.8)
 * 				ist.
 *
 * 			menueAktion[Y] = X;
 * 				Diesem String wird die Aktion X (siehe unten) zugewiesen, welche
 * 				durch Klicken auf den Menü-Eintrag Y ausgeführt wird.
 *
 * 				Aktionen:
 * 				----------
 * 				X = 0:
 * 				 - keine Aktion (Standard)
 *
 * 				X = 1:
 * 				 - springe eine Ebene tiefer
 *
 *				X = -1:
 * 				 - springe eine Ebene höher
 *
 * 				X = 2:
 * 				 - springe eine Ebene tiefer OHNE Einstellungen der Menü-Ebene zu
 * 				   löschen (, d.h. die deklarierten Variablen auf 0 zu setzen).
 *
 * 				X = -2:
 * 				 - springe eine Ebene höher OHNE Einstellungen des Menü-
 * 				   Eintrages zu löschen
 */

#ifndef MenueModul_h
#define MenueModul_h

#include <Arduino.h>
#include <LiquidCrystal.h>
#include "RotaryEncoder.h"


LiquidCrystal lcd(4,5,9,10,11,12);


byte menueEintragSprung = LOW;
byte menueFuehrungZustand = LOW;
byte menueRotaryEncoder = 1;
byte menueZurueckPfeil = 1;
int menueZeilenAnzahl = 1;
char* menueCursorZeichen;
char* menueEintrag[9][6];
int menueAktion[9];

long menueAdresse = 1;
byte menueCursorPos = 0;  // 0 = oben ; 1 = unten
int menueAuswahl = 1; 	 // Auswahl sagt auf welchem MenueEintrag[] Cursor ist
byte menueButtonAktion;
byte menueEinstellung = HIGH;



void menueReset(){
	menueEintragSprung = LOW;
	menueFuehrungZustand = LOW;
	menueRotaryEncoder = 1;
	menueZurueckPfeil = 1;
	menueZeilenAnzahl = 1;


	menueAuswahl = 1;
	menueCursorPos = 0;
	encoderPos = 0;
	lcd.clear();
	memset(menueEintrag, 0, sizeof menueEintrag);
	memset(menueAktion, 0, sizeof menueAktion);
	menueCursorZeichen = "";

	menueEinstellung = HIGH;
}

void menueEbeneHoeher(){
	long multiplier = 10;

	while(1){
		if(menueAdresse < multiplier*10){
			while(menueAdresse >= multiplier){
				menueAdresse -= multiplier;
			}
			break;
		}
		multiplier *= 10;
	}

	if(menueAdresse < 1) menueAdresse = 1;

	menueEinstellung = HIGH;
}

void menueEbeneTiefer(){
	long multiplier = 10;

	while(1){
		if(menueAdresse < multiplier){
			menueAdresse += (multiplier*menueAuswahl);
			break;
		}
		else{
			multiplier *= 10;
		}
	}
	menueEinstellung = HIGH;
}

void menueEintraegePrint(short Auswahl){
	for(byte i = 0; i <= 5; i++){
		lcd.print(menueEintrag[menueAuswahl+Auswahl][i]);
	}
}

void menueFuehrung(){
	if(menueFuehrungZustand == HIGH){
		if(menueRotaryEncoder == 1 && menueZeilenAnzahl+menueZurueckPfeil > 1){
			if(encoderPos >= 1 && menueCursorPos == 0){
				menueAuswahl++;
				menueCursorPos = 1;
				lcd.clear();
			}
			else if(encoderPos >= 1 && menueCursorPos == 1){
				menueAuswahl++;
				//CursorPos bleibt 1

				if(menueAuswahl > menueZeilenAnzahl+menueZurueckPfeil){  //Cursor ist schon ganz unten
					if(menueEintragSprung == HIGH){
						menueAuswahl = 1;
						menueCursorPos = 0;
						lcd.clear();
					}
					else{
						menueAuswahl = menueZeilenAnzahl+menueZurueckPfeil;
					}
				}
				else lcd.clear();
			}
			else if(encoderPos <= -1 && menueCursorPos == 0){
				menueAuswahl--;
				//CursorPos bleibt 0

				if(menueAuswahl < 1){  // Cursor ist schon ganz oben
					if(menueEintragSprung == HIGH){
						menueAuswahl = menueZeilenAnzahl+menueZurueckPfeil;
						menueCursorPos = 1;
						lcd.clear();
					}
					else{
						menueAuswahl = 1;
					}
				}
				else lcd.clear();
			}
			else if(encoderPos <= -1 && menueCursorPos == 1){
				menueAuswahl--;
				menueCursorPos = 0;
				lcd.clear();
			}
			encoderPos = 0;
		}

		//AUSGABEN///////////////////

		if(menueCursorPos == 0){
			lcd.setCursor(0,0);

			menueEintraegePrint(0);

			if(menueZeilenAnzahl+menueZurueckPfeil > 1){
				lcd.setCursor(0,1);
				menueEintraegePrint(1);
			}
		}
		else if(menueCursorPos == 1){
			lcd.setCursor(0,0);
			menueEintraegePrint(-1);

			if(menueZeilenAnzahl+menueZurueckPfeil > 1){
				lcd.setCursor(0,1);
				menueEintraegePrint(0);
			}
		}

		lcd.setCursor(15, menueCursorPos);
		lcd.print(menueCursorZeichen);

		/*Serial.print(encoderPos);
		Serial.print("  ");
		Serial.println(Auswahl); */

		/////////////////////////////

		if(menueZurueckPfeil == 1){
			menueEintrag[menueZeilenAnzahl+1][0] = "<==";
			menueAktion[menueZeilenAnzahl+1] = -1;
		}

		//////////////////////////////////////////////////////////////////////////

		encoderButtonRead();
		if(encoderButtonPressed == LOW){
			menueButtonAktion = HIGH;		//eine Aktion kann durch drücken des Knopfes durchgeführt werden
		}
		if(encoderButtonPressed == HIGH && menueButtonAktion == HIGH){
			if(menueAktion[menueAuswahl] == -1){		//Auswahl ist: in nächst höhere Ebene/Adresse
				//zurückkehren in übergeordnete Ebene
				menueEbeneHoeher();
				menueReset();
			}
			else if(menueAktion[menueAuswahl] == 1){	//Auswahl ist: in nächst tiefere Ebene/Adresse
				//tiefergehen in tiefere Ebene
				menueEbeneTiefer();
				menueReset();
			}
			else if(menueAktion[menueAuswahl] == 2){	//Auswahl ist: in nächst tiefere Ebene/Adresse OHNE menueClear()
				menueEbeneTiefer();
			}
			else if(menueAktion[menueAuswahl] == -2){	//Auswahl ist: in nächst höhere Ebene/Adresse OHNE menueClear()
				menueEbeneHoeher();
			}
			menueButtonAktion = LOW;
		}
	}
}

void menueBegin(){
	lcd.begin(16, 2);
}

#endif









