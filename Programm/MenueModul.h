/*
 *	Dies ist das Men�-Modul des Projektes "Arduino Uhr" der Jugendgruppe
 *	des OVs G11 Leverkusen von IGEL e.V. und DARC e.V. .
 *
 * 	geschrieben von Ralf Rumbler, DO3KV
 */

/********ERKLAERUNG DER FUNKTIONEN & VARIABLEN DES MODULS********
 *
 *
 *
 *		!!!! Diese Erkl�rung muss gruendlich �berarbeitet werden,
 *		da die Verwendung des Moduls sich sehr ver�ndert hat !!!!
 *
 *
 *
 *
 *
 *
 *
 * 		Funktionen:
 * 		==========================================================================
 * 			menueBegin()
 * 				Diese Funktion muss in der Setup() Funktion aufgerufen werden,
 * 				damit das Modul funktionsf�hig ist.
 *
 * 			menueFuehrung()
 * 				Diese Funktion muss am Ende der Loop() Funktion abgerufen werden,
 * 				damit Men�s gef�hrt werden k�nnen.
 *
 * 			menueReset()
 * 				Durch abrufen dieser Funktion werden alle Deklarationen/Zuweisungen
 * 				des aktuellen Men�s zur�ckgesetzt.
 *
 * 			menueEbeneTiefer()
 * 				Durch abrufen dieser Funktion wird zur n�chst tieferen Ebene
 * 				gesprungen (ohne menueReset() auszuf�hren), indem die menueAdresse
 * 				ver�ndert wird (siehe menueAdresse).
 *
 * 			menueEbeneHoeher()
 * 				Durch abrufen dieser Funktion wird zur n�chst h�heren Ebene
 * 				gesprungen (ohne menueReset() auszuf�hren), indem die menueAdresse
 * 				ver�ndert wird (siehe menueAdresse).
 *
 *
 *		ausgebende/Werte tragende  Variablen:
 * 		==========================================================================
 *			menueAdresse
 *				Diese Variable tr�gt die Adresse der aktuellen Ebene. Nach Starten
 *				des Programm ist sie als erstes 1.
 *
 *				springen in n�chst tiefere Ebene:
 *				----------------------------------
 *				Wenn in die n�chst tiefere Ebene gesprungen wird, wird  "[aktuelle
 *				Adresse] + 10^[Anzahl der Stellen der aktuellen Adresse]
 *				*menueAuswahl" gerechnet.
 *				Bsp.: Wenn die menueAdresse zuerst 1 ist, der 4. Men�punkt
 *				ausgew�hlt ist (menueAuswahl = 4) und in die n�chst tiefere Ebene
 *				gesprungen wird, ist die Adresse danach 41. Wenn danach der 3.
 *				Men�punkt ausgew�hlt ist und in die n�chst tiefere Ebene gesprungen
 *				wird, ist die Adresse danach 341 usw. .
 *
 *				springen in n�chst h�here Ebene:
 *				----------------------------------
 *				Wenn in die n�chst h�here Ebene gesprungen wird, wird die erste
 *				Stelle der Adresse der aktuellen Ebene abgezogen.
 *				Bsp.: Wenn menueAdresse zuerst 267 ist und in die n�chst h�here
 *				Ebene gesprungen wird, ist die Adresse danach 67.
 *
 *			menueAuswahl
 *				Diese Variable entspricht der Nummer des aktuell ausgew�hlten
 *				Men�-Eintrages.
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
 * 				Funktionen des Men�-Moduls.
 *
 * 			menueEintragSprung = HIGH/LOW;
 * 				Diese Deklaration aktiviert (HIGH), oder deaktiviert (LOW)...
 * 				1. das Springen zum ersten Eintrag, bei weiterdrehen des Rotary
 * 				Encoders, wenn der letzte Men�-Eintrag erreicht wurde.
 * 				2. das Springen zum letzten Eintrag bei, weiterdrehen des Rotary
 * 				Encoders, wenn der erste Men�-Eintrag erreicht wurde.
 *
 * 			menueRotaryEncoder = HIGH/LOW;
 * 				Diese Deklaration aktiviert (HIGH), oder deaktiviert (LOW), die
 * 				Benutzung vom Rotary Encoder vom Men�-Modul.
 *
 * 			menueZurueckPfeil = HIGH/LOW;
 * 				Diese Deklaration aktiviert (HIGH), oder deaktiviert (LOW), das
 * 				Hinzuf�gen von einem Pfeil unter den aktuellen Men�-Eintr�gen,
 * 				mit welchen man durch Klicken zur n�chst �bergeordneten Ebene
 * 				gelangen kann.
 *
 * 			menueZeilenAnzahl = X;
 * 				Durch die Deklaration dieser Variable wird die Anzahl der
 * 				Men�-Eintr�ge X (min. 0, max. 8 einschließlich Zur�ck-Pfeil Eintrag)
 * 				der Men�-Ebene festgelegt.
 *
 * 			menueCursorZeichen = X;
 * 				Diesem String wird das Zeichen X, welches als Cursor benutzt werden
 * 				soll, zugewiesen (wie z.B. "<").
 *
 *
 *
 * 			menueEintrag[Y][Z] = X;
 * 				Diesem String wird die Zeichenfolge X (max. 14 Zeichen) zugewiesen,
 * 				welche der Men�-Eintrag Z (min. 1, max. 6) in Zeile Y (min. 1, max.
 * 				7) ist.
 *
 * 			menueAktion[Y] = X;
 * 				Diesem String wird die Aktion X (siehe unten) zugewiesen, welche
 * 				durch Klicken auf den Men�-Eintrag Y ausgef�hrt wird.
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
 * 				 - springe eine Ebene h�her
 *
 * 				X = 2:
 * 				 - springe eine Ebene tiefer OHNE Einstellungen der Men�-Ebene zu
 * 				   l�schen (, d.h. die deklarierten Variablen auf 0 zu setzen).
 *
 * 				X = -2:
 * 				 - springe eine Ebene h�her OHNE Einstellungen des Men�-
 * 				   Eintrages zu l�schen
 */

#ifndef MenueModul_h
#define MenueModul_h

#include <Arduino.h>
#include "SmallLiquidCrystal.h"
#include "RotaryEncoder.h"


SmallLiquidCrystal lcd(4,5,9,10,11,12);


byte menueEintragSprung = LOW;
byte menueFuehrungZustand = LOW;
byte menueRotaryEncoder = 1;
byte menueHoeherPfeil = 1;
byte menueZeilenAnzahl = 1;
char menueCursorZeichen = ' ';
//char menueEintrag[9][17];
//char menueEintragZusatz[9][7][7];
//int menueAktion[9];
char menueText[17];
int menueAktion;


unsigned long menueAdresse = 1;
byte menueCursorPos = 0;  // 0 = oben ; 1 = unten
byte menueAuswahl = 1; 	 // Auswahl sagt auf welchem MenueEintrag[] Cursor ist
byte menueButtonAktion;
byte menueEinstellung = HIGH;
byte menueCursorBewegt = HIGH;

void menueReset(){
	menueEintragSprung = LOW;
	menueFuehrungZustand = LOW;
	menueRotaryEncoder = 1;
	menueHoeherPfeil = 1;
	menueZeilenAnzahl = 1;


	menueAuswahl = 1;
	menueCursorPos = 0;
	menueCursorBewegt = HIGH;
	encoderPos = 0;
	lcd.clear();
	//memset(menueEintrag, 0, sizeof menueEintrag);
	//memset(menueAktion, 0, sizeof menueAktion);
	menueCursorZeichen = ' ';

	menueEinstellung = HIGH;
}

void menueEbeneHoeher(){
	unsigned long multiplier = 10;

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

	menueCursorBewegt = HIGH;
	menueEinstellung = HIGH;
}
void menueEbeneTiefer(){
	unsigned long multiplier = 10;

	while(1){
		if(menueAdresse < multiplier){
			menueAdresse += (multiplier*menueAuswahl);
			break;
		}
		else{
			multiplier *= 10;
		}
	}

	menueCursorBewegt = HIGH;
	menueEinstellung = HIGH;
}

void menuePrint(byte Zeile, byte Spalte){
	if(menueCursorPos == 0){
		if(Zeile == menueAuswahl){
			lcd.setCursor(Spalte,0);
			lcd.print(menueText);
		}
		else if(Zeile == menueAuswahl+1){
			lcd.setCursor(Spalte,1);
			lcd.print(menueText);
		}
	}
	else{
		if(Zeile == menueAuswahl){
			lcd.setCursor(Spalte,1);
			lcd.print(menueText);
		}
		else if(Zeile == menueAuswahl-1){
			lcd.setCursor(Spalte,0);
			lcd.print(menueText);
		}
	}
}
void menueEintragErstellen(byte Zeile, int Aktion){
	menuePrint(Zeile, 0);

	if(menueAuswahl == Zeile) menueAktion = Aktion;
}
void menueEintragZusatzErstellen(byte Zeile, byte Spalte){
	menuePrint(Zeile, Spalte-1);
}

void menueFuehrung(){
	if(menueFuehrungZustand == HIGH){
		if(menueRotaryEncoder == 1 && menueZeilenAnzahl+menueHoeherPfeil > 1){
			if(encoderPos >= 1 && menueCursorPos == 0){
				menueAuswahl++;
				menueCursorPos = 1;
				lcd.setCursor(15, 0);
				lcd.print(" ");
			}
			else if(encoderPos >= 1 && menueCursorPos == 1){
				menueAuswahl++;
				//CursorPos bleibt 1

				if(menueAuswahl > menueZeilenAnzahl+menueHoeherPfeil){  //Cursor ist schon ganz unten
					if(menueEintragSprung == HIGH){
						menueAuswahl = 1;
						menueCursorPos = 0;
						lcd.clear();
					}
					else{
						menueAuswahl = menueZeilenAnzahl+menueHoeherPfeil;
					}
				}
				else lcd.clear();
			}
			else if(encoderPos <= -1 && menueCursorPos == 0){
				menueAuswahl--;
				//CursorPos bleibt 0

				if(menueAuswahl < 1){  // Cursor ist schon ganz oben
					if(menueEintragSprung == HIGH){
						menueAuswahl = menueZeilenAnzahl+menueHoeherPfeil;
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
				lcd.setCursor(15, 1);
				lcd.print(" ");
			}

			if(encoderPos != 0)	menueCursorBewegt = HIGH;

			encoderPos = 0;
		}

		//AUSGABEN///////////////////

		/*if(menueCursorPos == 0){
			lcd.setCursor(0,0);

			menueEintraegePrint(0);

			if(menueZeilenAnzahl+menueHoeherPfeil > 1){
				lcd.setCursor(0,1);
				menueEintraegePrint(1);
			}
		}
		else if(menueCursorPos == 1){
			lcd.setCursor(0,0);
			menueEintraegePrint(-1);

			if(menueZeilenAnzahl+menueHoeherPfeil > 1){
				lcd.setCursor(0,1);
				menueEintraegePrint(0);
			}
		}*/

		if(menueCursorZeichen != ' '){
			lcd.setCursor(15, menueCursorPos);
			lcd.print(menueCursorZeichen);
		}

		/*Serial.print(encoderPos);
		Serial.print("  ");
		Serial.println(Auswahl); */

		/////////////////////////////

		if(menueHoeherPfeil == 1){
			strcpy_P(menueText, (const char*) F("<=="));
			menueEintragErstellen(menueZeilenAnzahl+1, -1);
		}

		//////////////////////////////////////////////////////////////////////////

		encoderButtonRead();
		if(encoderButtonPressed == LOW){
			menueButtonAktion = HIGH;		//eine Aktion kann durch dr�cken des Knopfes durchgef�hrt werden
		}
		if(encoderButtonPressed == HIGH && menueButtonAktion == HIGH){
			if(menueAktion == -1){		//Auswahl ist: in n�chst h�here Ebene/Adresse
				menueEbeneHoeher();
				menueReset();
			}
			else if(menueAktion == 1){	//Auswahl ist: in n�chst tiefere Ebene/Adresse
				menueEbeneTiefer();
				menueReset();
			}
			else if(menueAktion == -2){	//Auswahl ist: in n�chst h�here Ebene/Adresse OHNE menueClear()
				menueEbeneHoeher();
			}
			else if(menueAktion == 2){	//Auswahl ist: in n�chst tiefere Ebene/Adresse OHNE menueClear()
				menueEbeneTiefer();
			}
			menueButtonAktion = LOW;
		}
	}
}

#endif



















