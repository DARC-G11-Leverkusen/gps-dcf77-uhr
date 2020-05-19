/*
 *	Dies ist das Wecker-Modul des Projektes "Arduino Uhr" der Jugendgruppe
 *	des OVs G11 Leverkusen von IGEL e.V. und DARC e.V. .
 *
 * 	geschrieben von Ralf Rumbler, DO3KV
 *
 *
 *	vom Modul benötigte EEPROM-Adressen: 0 bis 34
 */

#ifndef WeckerModul_h
#define WeckerModul_h

#include <Arduino.h>
#include <EEPROM.h>

#include "UebergreifendeProgramme.h"
#include "ZeitModul.h"

byte weckerDaten[5][7];
/*	Belegung von weckerDaten:
 * 	==========================
 *	weckerDaten[weckerAuswahl][0] >> Zustand >> 1 = AN ; 0 = AUS
 * 	weckerDaten[weckerAuswahl][1] >> Zeit: Stunden >> ab 1
 * 	weckerDaten[weckerAuswahl][2] >> Zeit: Minuten >> ab 1
 * 	weckerDaten[weckerAuswahl][3] >> Wiederholen >> 0 = AUS ; 1 = ...
 * 	weckerDaten[weckerAuswahl][4] >> Nummer vom ausgewählten Ton >> ab 1 ; bis 3
 * 	anzahlToene >> Anzahl der eingespeicherten Töne
 * 	weckerDaten[weckerAuswahl][5] >> Lautstärke Ton in %
 * 	weckerDaten[weckerAuswahl][6] >> zu Wiederholende Tage (in Bits)
 */
byte weckerAuswahl;
byte weckerButtonStateA = LOW;
byte weckerButtonStateB = LOW;
byte weckerEEPROMwriteState = LOW;
byte weckerWriteDaten;
byte weckerAnzahlToene;
byte weckerHatGeklingelt;

///////////////////////////////////////////////////////////////////////////////////

void weckerEEPROMwrite(){
	EEPROM.write((weckerAuswahl*7)+weckerWriteDaten, weckerDaten[weckerAuswahl][weckerWriteDaten]);
	if(weckerWriteDaten == 1){
		EEPROM.write((weckerAuswahl*7)+2, weckerDaten[weckerAuswahl][2]);
	}
	else if(weckerWriteDaten == 3){
		EEPROM.write((weckerAuswahl*7)+6, weckerDaten[weckerAuswahl][6]);
	}

	//Ausgabe für Fehlerbehebung////////
	/*Serial.print("EEPROM write ");

	Serial.print((weckerAuswahl*7)+weckerWriteDaten);
	Serial.print(" ");
	Serial.print(weckerDaten[weckerAuswahl][weckerWriteDaten]);

	if(weckerWriteDaten == 1){
		Serial.print(" ; ");
		Serial.print((weckerAuswahl*7)+2);
		Serial.print(" ");
		Serial.print(weckerDaten[weckerAuswahl][2]);
	}
	else if(weckerWriteDaten == 3){
		Serial.print(" ; ");
		Serial.print((weckerAuswahl*7)+6);
		Serial.print(" ");
		Serial.print(weckerDaten[weckerAuswahl][6]);
	}
	Serial.println();*/
	////////////////////////////////////
}
void weckerEEPROMread(){
	for(byte a = 0; a <= 4; a++){
		for(byte b = 0; b <= 6; b++){
			weckerDaten[a][b] = EEPROM.read((a*7)+b);
		}
	}

	//Ausgabe für Fehlerbehebung////////
	//Serial.println("EEPROM read");
	////////////////////////////////////
}

/////////////////////////////////////////////////////

void weckerAusgabeZustand(){
	if(weckerDaten[weckerAuswahl][0] == 1){
		strcpy_P(menueText, (const char*) F(" AN"));
		menueEintragZusatzErstellen(1, 12);
	}
	else{
		strcpy_P(menueText, (const char*) F("AUS"));
		menueEintragZusatzErstellen(1, 12);
	}
}
void weckerAusgabeZeit(){
	/*memset(itoaBufferA, 0, sizeof itoaBufferA);
	memset(itoaBufferB, 0, sizeof itoaBufferB);

	itoa(weckerDaten[weckerAuswahl][1], itoaBufferA, 10);
	strcpy(menueText, itoaBufferA);

	if(weckerDaten[weckerAuswahl][1] < 10) menueEintragZusatzErstellen(2, 11);
	else 								   menueEintragZusatzErstellen(2, 10);

	strcpy_P(menueText, (const char*) F(":"));
	menueEintragZusatzErstellen(2, 12);

	itoa(weckerDaten[weckerAuswahl][2], itoaBufferB, 10);
	strcpy(menueText, itoaBufferB);

	if(weckerDaten[weckerAuswahl][2] < 10){
		menueEintragZusatzErstellen(2, 14);
		strcpy_P(menueText, (const char*) F("0"));
		menueEintragZusatzErstellen(2, 13);
	}
	else{
		menueEintragZusatzErstellen(2, 13);
	}*/

	ausgabeUhrzeit(weckerDaten[weckerAuswahl][1], weckerDaten[weckerAuswahl][2], 2, 10, 0);
}
/*void weckerAusgabeWiederholen(){
	if(weckerDaten[weckerAuswahl][3] == 1){
		menueEintrag[3][1] = "...";
	}
	else{
		menueEintrag[3][1] = "AUS";
	}
}*/
void weckerAusgabeTon(){
	memset(itoaBufferC, 0, sizeof itoaBufferC);

	if(weckerDaten[weckerAuswahl][4] > 0){
		strcpy_P(menueText, (const char*) F("Nr."));
		menueEintragZusatzErstellen(4, 10);

		itoa(weckerDaten[weckerAuswahl][4], itoaBufferC, 10);
		strcpy(menueText, itoaBufferC);
		menueEintragZusatzErstellen(4, 14);
	}
	else{
		strcpy_P(menueText, (const char*) F("-"));
		menueEintragZusatzErstellen(4, 14);
	}
}
void weckerAusgabeLautstaerke(){
	//memset(itoaBufferD, 0, sizeof itoaBufferD);

	strcpy_P(menueText, (const char*) F("  "));
	menueEintragZusatzErstellen(5, 11);

	itoa(weckerDaten[weckerAuswahl][5], itoaBufferD, 10);
	strcpy(menueText, itoaBufferD);

	if(weckerDaten[weckerAuswahl][5] < 10){
		menueEintragZusatzErstellen(5, 13);
	}
	else if(weckerDaten[weckerAuswahl][5] < 100){
		menueEintragZusatzErstellen(5, 12);
	}
	else{
		menueEintragZusatzErstellen(5, 11);
	}

	strcpy_P(menueText, (const char*) F("%"));
	menueEintragZusatzErstellen(5, 14);
}

///////////////////////////////////

void weckerEbeneA(){
	if(menueEinstellung == HIGH){
		menueFuehrungZustand = HIGH;
		menueEintragSprung = LOW;
		menueRotaryEncoder = HIGH;
		menueHoeherPfeil = HIGH;
		menueZeilenAnzahl = 5;
		menueCursorZeichen = '<';

		/*strcpy_P(menueEintrag[1], (const char*) F("Wecker A    =>"));
		strcpy_P(menueEintrag[2], (const char*) F("Wecker B    =>"));
		strcpy_P(menueEintrag[3], (const char*) F("Wecker C    =>"));
		strcpy_P(menueEintrag[4], (const char*) F("Wecker D    =>"));
		strcpy_P(menueEintrag[5], (const char*) F("Wecker E    =>"));*/

		menueEinstellung = LOW;
	}

	if(menueCursorBewegt == HIGH){
		for(byte i = 1; i <= 5; i++){
			strcpy_P(menueText, (const char*) F("Wecker"));
			menueEintragErstellen(i, 0);

			itoa(i, itoaBufferA, 10);
			strcpy(menueText, (const char*) itoaBufferA);
			menueEintragZusatzErstellen(i, 8);

			strcpy_P(menueText, (const char*) F("=>"));
			menueEintragZusatzErstellen(i, 13);

			menueCursorBewegt = LOW;
		}
	}

	if(encoderButtonPressed == LOW){
		weckerButtonStateA = HIGH;
	}
	if(encoderButtonPressed == HIGH && weckerButtonStateA == HIGH){
		weckerAuswahl = menueAuswahl-1;
		menueReset();
		menueAdresse = 1111;
	}
}
void weckerEbeneB(){
	if(menueEinstellung == HIGH){
		if(weckerEEPROMwriteState == HIGH){
			weckerEEPROMwrite();
			weckerEEPROMwriteState = LOW;
		}
		menueFuehrungZustand = HIGH;
		menueEintragSprung = LOW;
		menueRotaryEncoder = HIGH;
		menueHoeherPfeil = HIGH;
		menueCursorZeichen = '<';

		menueZeilenAnzahl = 5;

		menueEinstellung = LOW;
	}

	if(menueCursorBewegt == HIGH){
		strcpy_P(menueText, (const char*) F("Zustand:"));
		menueEintragErstellen(1, 2);
		strcpy_P(menueText, (const char*) F("Zeit:"));
		menueEintragErstellen(2, 2);
		strcpy_P(menueText, (const char*) F("Wiederholen =>"));
		menueEintragErstellen(3, 1);
		strcpy_P(menueText, (const char*) F("Ton:"));
		menueEintragErstellen(4, 1);
		strcpy_P(menueText, (const char*) F("Lautst.:"));
		menueEintragErstellen(5, 2);

		weckerAusgabeZustand();
		weckerAusgabeZeit();
		//weckerAusgabeWiederholen();
		weckerAusgabeTon();
		weckerAusgabeLautstaerke();

		menueCursorBewegt = LOW;
	}

	weckerButtonStateA = LOW;
}
void weckerEbeneC_Zustand(){
	if(menueEinstellung == HIGH){
		menueRotaryEncoder = LOW;
		menueCursorZeichen = 'X';
		menueAktion = -1;

		encoderPos = 0;

		weckerEEPROMwriteState = HIGH;
		weckerWriteDaten = 0;

		menueEinstellung = LOW;
	}

	if(encoderPos != 0){
		if(weckerDaten[weckerAuswahl][0] == 0){
			weckerDaten[weckerAuswahl][0] = 1;
		}
		else{
			weckerDaten[weckerAuswahl][0] = 0;
		}
		encoderPos = 0;

		weckerAusgabeZustand();
	}
}
void weckerEbeneC_Zeit(){
	if(menueEinstellung == HIGH){
		menueRotaryEncoder = LOW;
		menueCursorZeichen = 'X';

		menueAktion = 2;

		encoderPos = weckerDaten[weckerAuswahl][1];
		encoderChanged = LOW;

		weckerEEPROMwriteState = HIGH;
		weckerWriteDaten = 1;

		menueEinstellung = LOW;
	}

	if(encoderPos > 23){
		encoderPos = 0;
	}
	if(encoderPos < 0){
		encoderPos = 23;
	}

	weckerDaten[weckerAuswahl][1] = encoderPos;

	if(encoderChanged == HIGH){
		weckerAusgabeZeit();
		encoderChanged = LOW;
	}
}
void weckerEbeneD_Zeit(){
	if(menueEinstellung == HIGH){
		menueAktion = 2;

		encoderPos = weckerDaten[weckerAuswahl][2];
		encoderChanged = LOW;

		menueEinstellung = LOW;
	}

	if(encoderPos > 59){
		encoderPos = 0;
	}
	if(encoderPos < 0){
		encoderPos = 59;
	}

	weckerDaten[weckerAuswahl][2] = encoderPos;

	if(encoderChanged == HIGH){
		weckerAusgabeZeit();
		encoderChanged = LOW;
	}
}
void weckerEbeneC_Wiederholen(){
	byte zaeler = 0;

	if(menueEinstellung == HIGH){
		menueFuehrungZustand = HIGH;
		menueEintragSprung = LOW;
		menueRotaryEncoder = HIGH;
		menueHoeherPfeil = HIGH;
		menueCursorZeichen = '<';

		menueZeilenAnzahl = 7;

		weckerWriteDaten = 3;
		weckerEEPROMwriteState = HIGH;	//kann unnötig sein

		menueEinstellung = LOW;

		goto weckerLabel1;
	}

	if(menueCursorBewegt == HIGH){
		weckerLabel1:

		strcpy_P(menueText, (const char*) F("Mo"));
		menueEintragErstellen(1, 0);
		strcpy_P(menueText, (const char*) F("Di"));
		menueEintragErstellen(2, 0);
		strcpy_P(menueText, (const char*) F("Mi"));
		menueEintragErstellen(3, 0);
		strcpy_P(menueText, (const char*) F("Do"));
		menueEintragErstellen(4, 0);
		strcpy_P(menueText, (const char*) F("Fr"));
		menueEintragErstellen(5, 0);
		strcpy_P(menueText, (const char*) F("Sa"));
		menueEintragErstellen(6, 0);
		strcpy_P(menueText, (const char*) F("So"));
		menueEintragErstellen(7, 0);

		for(short i = 1; i <= 7; i++){
			strcpy_P(menueText, (const char*) F("["));
			menueEintragZusatzErstellen(i, 12);

			if(bitRead(weckerDaten[weckerAuswahl][6], i-1) == 0){
				strcpy_P(menueText, (const char*) F(" "));
				menueEintragZusatzErstellen(i, 13);
			}
			else{
				strcpy_P(menueText, (const char*) F("X"));
				menueEintragZusatzErstellen(i, 13);
			}

			zaeler += bitRead(weckerDaten[weckerAuswahl][6], i-1);

			if(zaeler > 0) weckerDaten[weckerAuswahl][3] = 1;
			else weckerDaten[weckerAuswahl][3] = 0;

			strcpy_P(menueText, (const char*) F("]"));
			menueEintragZusatzErstellen(i, 14);
		}

		menueCursorBewegt = LOW;
	}

	if(encoderButtonPressed == LOW){
		weckerButtonStateA = HIGH;
	}
	if(encoderButtonPressed == HIGH && weckerButtonStateA == HIGH){
		menueAdresse = 131111;
	}
}
void weckerEbeneD_Wiederholen(){
	if(bitRead(weckerDaten[weckerAuswahl][6], menueAuswahl-1) == 1){
		bitWrite(weckerDaten[weckerAuswahl][6], menueAuswahl-1, 0);
	}
	else{
		bitWrite(weckerDaten[weckerAuswahl][6], menueAuswahl-1, 1);
	}

	weckerEEPROMwrite();

	weckerButtonStateA = LOW;

	menueEinstellung = HIGH;
	menueAdresse = 31111;
}
void weckerEbeneC_Ton(){
	if(menueEinstellung == HIGH){
		menueFuehrungZustand = HIGH;
		menueEintragSprung = LOW;
		menueRotaryEncoder = HIGH;
		menueHoeherPfeil = HIGH;
		menueCursorZeichen = '<';

		weckerEEPROMwriteState = HIGH;
		weckerWriteDaten = 4;

		menueEinstellung = LOW;
	}

	if(menueCursorBewegt == HIGH){
		if(weckerAnzahlToene == 0){
			menueZeilenAnzahl = 1;

			strcpy_P(menueText, (const char*) F("*leer*"));
			menueEintragErstellen(1, 0);
		}
		/*else{
			menueZeilenAnzahl = weckerAnzahlToene;
			for(byte i = 1; i <= weckerAnzahlToene; i++){
				strcpy_P(menueEintrag[i], (const char*) weckerTonName[i-1]);
				strcpy_P(menueEintrag[i], (const char*) F("["));
				strcpy_P(menueEintrag[i], (const char*) F(" "));
				strcpy_P(menueEintrag[i], (const char*) F("]"));
			}
			strcpy_P(menueEintragZusatz[weckerDaten[weckerAuswahl][4]][2], (const char*) F("X"));
		}*/
	}

	if(weckerAnzahlToene != 0){
		if(encoderButtonPressed == LOW){
			weckerButtonStateA = HIGH;
		}
		if(encoderButtonPressed == HIGH && weckerButtonStateA == HIGH){
			menueAdresse = 141111;
		}
	}
}
void weckerEbeneD_Ton(){
	weckerDaten[weckerAuswahl][4] = menueAuswahl;
	weckerButtonStateA = LOW;
	menueEinstellung = HIGH;
	menueAdresse = 41111;
}
void weckerEbeneC_Lautstaerke(){
	if(menueEinstellung == HIGH){
		menueRotaryEncoder = LOW;
		menueCursorZeichen = 'X';

		menueAktion = 2;

		encoderPos = weckerDaten[weckerAuswahl][5] / 10;
		encoderChanged = LOW;

		weckerEEPROMwriteState = HIGH;
		weckerWriteDaten = 5;

		menueEinstellung = LOW;
	}

	if(encoderPos > 10){
		encoderPos = 0;
	}
	if(encoderPos < 0){
		encoderPos = 10;
	}

	weckerDaten[weckerAuswahl][5] = encoderPos*10;

	if(encoderChanged == HIGH){
		weckerAusgabeLautstaerke();
		encoderChanged = LOW;
	}
}

void weckerKlingelnA(){
	if(menueEinstellung == HIGH){
		menueFuehrungZustand = HIGH;
		menueRotaryEncoder = LOW;
		menueHoeherPfeil = LOW;
		menueZeilenAnzahl = 2;

		/*	menueEintrag[1][1] = " Wecker ";
		 *	menueAktion[1] = 1;//" 10:10  ";
		 */

		menueAktion = 1;

		encoderChanged = LOW;

		menueEinstellung = LOW;
	}

	/*itoa(zeitStunden, itoaBufferA, 10);
	itoa(zeitMinuten, itoaBufferB, 10);

	strcpy_P(menueText, (const char*) itoaBufferA);
	if(zeitStunden <= 9) menueEintragZusatzErstellen(1, 7);
	else				 menueEintragZusatzErstellen(1, 6);

	strcpy_P(menueText, (const char*) F(":"));
	menueEintragZusatzErstellen(1, 8);


	if(zeitMinuten <= 9){
		strcpy_P(menueText, (const char*) F("0"));
		menueEintragZusatzErstellen(1, 9);

		strcpy_P(menueText, (const char*) itoaBufferB);
		menueEintragZusatzErstellen(1, 10);
	}
	else{
		strcpy_P(menueText, (const char*) itoaBufferB);
		menueEintragZusatzErstellen(1, 9);
	}*/

	ausgabeUhrzeit(zeitStunden, zeitMinuten, 1, 6, 0);


	if(intHalbeSekunde == 1){
		strcpy_P(menueText, (const char*) F("XXXX"));
		menueEintragErstellen(1, 0);
		menueEintragZusatzErstellen(1, 13);

		strcpy_P(menueText, (const char*) F("XXXXXXXXXXXXXXXX"));
		menueEintragErstellen(2, 0);
	}
	else{
		strcpy_P(menueText, (const char*) F("    "));
		menueEintragErstellen(1, 0);
		menueEintragZusatzErstellen(1, 13);

		strcpy_P(menueText, (const char*) F("                "));
		menueEintragErstellen(2, 0);
	}

	if(encoderButtonPressed == LOW) weckerButtonStateB = HIGH;

	if(encoderButtonPressed == HIGH && weckerButtonStateB == HIGH){
		menueReset();
		menueAdresse = 1;

		weckerButtonStateB = LOW;
	}
}

/////////////////////////////////////////////////////

void weckerFuehrung(){
	byte weckerTagUmformung;
	if(zeitWochentag >= 2 && zeitWochentag <= 7){
		weckerTagUmformung = zeitWochentag-2;
	}
	else if(zeitWochentag == 1){
		weckerTagUmformung = 6;
	}
	else{
		weckerTagUmformung = 7;
	}

	for(byte i = 0; i <= 4; i++){
		if(menueAdresse == 1 &&
		   weckerDaten[i][0] == 1 &&
		   weckerDaten[i][1] == zeitStunden){
			if(weckerDaten[i][2] == zeitMinuten){

				if(weckerDaten[i][3] == 0){
					weckerDaten[i][0] = 0;
					weckerWriteDaten = 0;
					weckerEEPROMwrite();

					menueReset();
					menueAdresse = 2;
				}
				else if(bitRead(weckerHatGeklingelt, i) == LOW &&
						weckerTagUmformung != 7 &&
						bitRead(weckerDaten[i][6], weckerTagUmformung) == 1){

					bitWrite(weckerHatGeklingelt, i, HIGH);

					menueReset();
					menueAdresse = 2;
				}
			}
			else if(weckerDaten[i][2] != zeitMinuten){
				bitWrite(weckerHatGeklingelt, i, LOW);
			}
		}
	}
}

#endif














