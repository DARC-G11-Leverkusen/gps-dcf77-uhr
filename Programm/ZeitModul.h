/*
 *	Dies ist das Zeit-Modul des Projektes "Arduino Uhr" der Jugendgruppe
 *	des OVs G11 Leverkusen von IGEL e.V. und DARC e.V. .
 *
 * 	geschrieben von Ralf Rumbler, DO3KV
 *
 *
 *	vom Modul benötigte EEPROM-Adressen: 35 bis 38
 */

#ifndef ZeitModul_h
#define ZeitModul_h

#include <Arduino.h>
#include "GpsModul.h"
#include "InterneZeitModul.h"
#include "UebergreifendeProgramme.h"

int zeitZeitzone;
byte zeitSekunden, zeitStunden, zeitMinuten, zeitTag = 1, zeitMonat = 1;
int zeitJahr = 2000;
int zeitWochentag;
byte zeitUhrzeitEingebenStatus, zeitDatumEingebenStatus;
byte zeitUhrzeitEingebenFertig, zeitDatumEingebenFertig;
byte zeitSommerzeitZustand;
byte zeitButtonStateB;
byte zeitSommerzeitBeginnTag, zeitSommerzeitEndTag, zeitSommerzeitEndTagChange = HIGH;

///////////////////////////////////////////////////////////////////////////////////

void zeitSommerzeitTagCheck(){
	//MEZ/MESZ/////////////////////////////////////
	for(byte i = 0; i <= 6; i ++){
		if(dayofweek(31-i, 3, zeitJahr) == 1){
			zeitSommerzeitBeginnTag = 31-i;
		}
		if(dayofweek(31-i, 10, zeitJahr) == 1){
			zeitSommerzeitEndTag = 31-i;
		}
	}
	zeitSommerzeitEndTagChange = HIGH;
	///////////////////////////////////////////////
}

void zeitEEPROMread(){
	zeitQuelle = EEPROM.read(35);
	zeitZeitzone = eepromReadInt(36);
	zeitSommerzeitZustand = EEPROM.read(38);
}

void zeitHalbesDatumAusgabe(byte a, byte b, byte c){
	strcpy_P(menueText, (const char*) F("."));
	menueEintragZusatzErstellen(2, a);

	strcpy(menueText, itoaBufferD);
	menueEintragZusatzErstellen(2, b);

	strcpy_P(menueText, (const char*) F("."));
	menueEintragZusatzErstellen(2, c);
}

/////////////////////////////////////////////////////

void zeitAusgabeZeitzone(){
	itoa(zeitZeitzone, itoaBufferA, 10);
	strcpy(menueText, itoaBufferA);

	if(zeitZeitzone == 0){
		menueEintragZusatzErstellen(3, 14);

		strcpy_P(menueText, (const char*) F(" "));
		menueEintragZusatzErstellen(3, 13);
	}

	else if(zeitZeitzone >= 1 && zeitZeitzone <= 9){
		menueEintragZusatzErstellen(3, 14);

		strcpy_P(menueText, (const char*) F(" +"));
		menueEintragZusatzErstellen(3, 12);
	}

	else if(zeitZeitzone >= 10){
		menueEintragZusatzErstellen(3, 13);

		strcpy_P(menueText, (const char*) F("+"));
		menueEintragZusatzErstellen(3, 12);
	}

	else if(zeitZeitzone <= -1 && zeitZeitzone >= -9){
		menueEintragZusatzErstellen(3, 13);

		strcpy_P(menueText, (const char*) F(" "));
		menueEintragZusatzErstellen(3, 12);
	}

	else if(zeitZeitzone <= -10) menueEintragZusatzErstellen(3, 12);
}
void zeitAusgabeQuelle(){
	switch(zeitQuelle){
	case 0:{
		strcpy_P(menueText, (const char*) F("  Auto"));
		menueEintragZusatzErstellen(1, 9);
	}break;
	case 1:{
		strcpy_P(menueText, (const char*) F("Intern"));
		menueEintragZusatzErstellen(1, 9);
	}break;
	case 2:{
		strcpy_P(menueText, (const char*) F("   GPS"));
		menueEintragZusatzErstellen(1, 9);
	}break;
	}
}
void zeitAusgabeEingebenUhrzeit(byte blink){
	itoa(zeitStunden, itoaBufferA, 10);
	itoa(zeitMinuten, itoaBufferB, 10);

	/*if(zeitStunden < 10) strcpy_P(menueEintragZusatz[1][1], (const char*) F(" "));
	else strcpy_P(menueEintragZusatz[1][1], (const char*) F(""));
	strcpy(menueEintragZusatz[1][2], itoaBufferA);
	if(zeitMinuten < 10) strcpy_P(menueEintragZusatz[1][4], (const char*) F("0"));
	else strcpy_P(menueEintragZusatz[1][4], (const char*) F(""));
	strcpy(menueEintragZusatz[1][5], itoaBufferB);*/

	ausgabeUhrzeit(zeitStunden, zeitMinuten, 1, 10, 1);

	if(intHalbeSekunde == 0 || blink == LOW){
		strcpy_P(menueText, (const char*) F(":"));
		menueEintragZusatzErstellen(1, 12);
	}
	else if(intHalbeSekunde == 1 && blink == HIGH){
		strcpy_P(menueText, (const char*) F(" "));
		menueEintragZusatzErstellen(1, 12);
	}
}
void zeitAusgabeEingebenDatum(){
	if(zeitTag < 1) zeitTag = 1;
	if(zeitMonat < 1) zeitMonat = 1;

	itoa(zeitTag, itoaBufferC, 10);
	itoa(zeitMonat, itoaBufferD, 10);
	itoa(zeitJahr, itoaIntBufferA, 10);

	strcpy(menueText, itoaBufferC);
	if(zeitTag <= 9 && zeitMonat <= 9){
		menueEintragZusatzErstellen(2, 7);

		zeitHalbesDatumAusgabe(8, 9, 10);

		strcpy_P(menueText, (const char*) F("  "));
		menueEintragZusatzErstellen(2, 5);
	}
	else if(zeitTag >= 10 && zeitMonat <= 9){
		menueEintragZusatzErstellen(2, 6);

		zeitHalbesDatumAusgabe(8, 9, 10);

		strcpy_P(menueText, (const char*) F(" "));
		menueEintragZusatzErstellen(2, 5);
	}
	else if(zeitTag <= 9 && zeitMonat >= 10){
		menueEintragZusatzErstellen(2, 6);

		zeitHalbesDatumAusgabe(7, 8, 10);

		strcpy_P(menueText, (const char*) F(" "));
		menueEintragZusatzErstellen(2, 5);
	}
	else{
		menueEintragZusatzErstellen(2, 5);

		zeitHalbesDatumAusgabe(7, 8, 10);
	}

	strcpy(menueText, itoaIntBufferA);
	menueEintragZusatzErstellen(2, 11);
	/*if(zeitJahr <= 9){
		menueEintragZusatzErstellen(2, 14);
		strcpy_P(menueText, (const char*) F("000"));
	}
	else if(zeitJahr <= 99){
		menueEintragZusatzErstellen(2, 13);
		strcpy_P(menueText, (const char*) F("00"));
	}
	else if(zeitJahr <= 999){
		menueEintragZusatzErstellen(2, 12);
		strcpy_P(menueText, (const char*) F("0"));
	}*/
}
void zeitAusgabeSommerzeit(){
	if(zeitSommerzeitZustand == 1) 		strcpy_P(menueText, (const char*) F("MEZ"));
	else if(zeitSommerzeitZustand == 2) strcpy_P(menueText, (const char*) F("aus"));
	menueEintragZusatzErstellen(2, 12);
}

///////////////////////////////////

void zeitEbeneA(){
	if(menueEinstellung == HIGH){
		menueFuehrungZustand = HIGH;
		menueEintragSprung = LOW;
		menueRotaryEncoder = HIGH;
		menueHoeherPfeil = HIGH;
		menueCursorZeichen = '<';

		if(zeitQuelle == 0) menueZeilenAnzahl = 4;
		else menueZeilenAnzahl = 3;

		menueEinstellung = LOW;
	}

	if(menueCursorBewegt == HIGH){
		strcpy_P(menueText, (const char*) F("Quelle: "));
		menueEintragErstellen(1, 2);
		strcpy_P(menueText, (const char*) F("Sommerzeit:"));
		menueEintragErstellen(2, 2);
		if(zeitQuelle != 2){
			strcpy_P(menueText, (const char*) F("Eingeben"));
			menueEintragErstellen(4-zeitQuelle, 1);
			strcpy_P(menueText, (const char*) F("=>"));
			menueEintragZusatzErstellen(4-zeitQuelle, 13);
		}
		if(zeitQuelle != 1){
			strcpy_P(menueText, (const char*) F("Zeitzone:"));
			menueEintragErstellen(3, 2);

			zeitAusgabeZeitzone();
		}

		zeitAusgabeQuelle();
		zeitAusgabeSommerzeit();

		menueCursorBewegt = LOW;
	}
}
void zeitEbeneB_Quelle(){
	if(menueEinstellung == HIGH){
		menueRotaryEncoder = LOW;
		menueCursorZeichen = 'X';
		menueAktion = 2;

		encoderChanged = LOW;
		encoderPos = zeitQuelle;

		menueEinstellung = LOW;
	}
	if(encoderPos > 2) encoderPos = 0;
	if(encoderPos < 0) encoderPos = 2;

	zeitQuelle = encoderPos;

	if(encoderChanged == HIGH){
		zeitAusgabeQuelle();
		encoderChanged = LOW;
	}
}
void zeitEbeneC_Quelle(){
	EEPROM.write(35, zeitQuelle);
	encoderPos = 0;
	menueAdresse = 211;
}
void zeitEbeneB_Weiterleitung(){
	if(zeitQuelle != 1) menueEbeneTiefer();
	else menueAdresse = 4211;
}
void zeitEbeneC_Zeitzone(){
	if(menueEinstellung == HIGH){
		menueRotaryEncoder = LOW;
		menueCursorZeichen = 'X';
		menueAktion = 2;

		encoderChanged = LOW;
		encoderPos = zeitZeitzone;

		menueEinstellung = LOW;
	}

	if(encoderPos < -12) encoderPos = -12;
	if(encoderPos > 12)  encoderPos = 12;

	zeitZeitzone = encoderPos;

	if(encoderChanged == HIGH){
		zeitAusgabeZeitzone();
		encoderChanged = LOW;
	}
}
void zeitEbeneD_Zeitzone(){
	eepromWriteInt(36, zeitZeitzone);
	encoderPos = 0;
	menueAdresse = 211;
}
void zeitEbeneB_Sommerzeit(){
	if(menueEinstellung == HIGH){
		menueRotaryEncoder = LOW;
		menueCursorZeichen = 'X';

		encoderChanged = LOW;
		encoderPos = zeitSommerzeitZustand;

		menueEinstellung = LOW;
	}

	if(encoderPos < 1) encoderPos = 2;
	if(encoderPos > 2) encoderPos = 1;

	zeitSommerzeitZustand = encoderPos;

	if(encoderChanged == HIGH){
		zeitAusgabeSommerzeit();
		encoderChanged = LOW;
	}
}
void zeitEbeneC_Sommerzeit(){
	EEPROM.write(38, zeitSommerzeitZustand);
	encoderPos = 0;
	menueAdresse = 211;
}
void zeitEbeneB_Eingeben(){
	if(menueEinstellung == HIGH){
		menueFuehrungZustand = HIGH;
		menueEintragSprung = LOW;
		menueRotaryEncoder = HIGH;
		menueHoeherPfeil = HIGH;
		menueZeilenAnzahl = 2;
		menueCursorZeichen = '<';

		zeitUhrzeitEingebenStatus = LOW;
		zeitDatumEingebenStatus   = LOW;

		menueEinstellung = LOW;
	}

	if(menueCursorBewegt == HIGH){
		strcpy_P(menueText, (const char*) F("Uhrzeit:"));
		menueEintragErstellen(1, 2);
		strcpy_P(menueText, (const char*) F("D.: "));
		menueEintragErstellen(2, 2);

		menueCursorBewegt = LOW;
	}
	zeitAusgabeEingebenUhrzeit(1);
	zeitAusgabeEingebenDatum();
}
void zeitEbeneC_EingebenUhrzeit(){
	if(menueEinstellung == HIGH){
		menueRotaryEncoder = LOW;
		menueCursorZeichen = 'X';

		encoderChanged = LOW;
		encoderPos = zeitStunden;

		zeitUhrzeitEingebenStatus = HIGH;

		menueEinstellung = LOW;
	}

	if(encoderPos > 23) encoderPos = 0;
	if(encoderPos < 0) encoderPos = 23;

	zeitStunden = encoderPos;

	if(encoderChanged == HIGH){
		zeitAusgabeEingebenUhrzeit(0);
		encoderChanged = LOW;
	}
}
void zeitEbeneD_EingebenUhrzeit(){
	if(menueEinstellung == HIGH){
		encoderChanged = LOW;
		encoderPos = zeitMinuten;

		menueEinstellung = LOW;
	}

	if(encoderPos > 59) encoderPos = 0;
	if(encoderPos < 0) encoderPos = 59;

	zeitMinuten = encoderPos;

	if(encoderChanged == HIGH){
		zeitAusgabeEingebenUhrzeit(0);
		encoderChanged = LOW;
	}

}
void zeitEbeneE_EingebenUhrzeit(){
	if(zeitQuelle == 1){
		TCNT1 = 34286;
		intHalbeSekunde = 0;
		intZeitSekunden = 0;
	}
	zeitUhrzeitEingebenFertig = HIGH;
	encoderPos = 0;
	menueAdresse = 4211;
}
void zeitEbeneC_EingebenDatum(){
	if(menueEinstellung == HIGH){
		menueRotaryEncoder = LOW;
		menueCursorZeichen = 'X';

		encoderChanged = LOW;
		encoderPos = zeitTag;

		zeitDatumEingebenStatus = HIGH;

		menueEinstellung = LOW;
	}

	if(encoderPos > 31) encoderPos = 1;
	if(encoderPos < 1) encoderPos = 31;

	zeitTag = encoderPos;

	if(encoderChanged == HIGH){
		zeitAusgabeEingebenDatum();
		encoderChanged = LOW;
	}
	zeitAusgabeEingebenUhrzeit(1);
}
void zeitEbeneD_EingebenDatum(){
	if(menueEinstellung == HIGH){
		encoderChanged = LOW;
		encoderPos = zeitMonat;

		menueEinstellung = LOW;
	}

	if(encoderPos > 12) encoderPos = 1;
	if(encoderPos < 1) encoderPos = 12;

	zeitMonat = encoderPos;

	if(encoderChanged == HIGH){
		zeitAusgabeEingebenDatum();

		encoderChanged = LOW;
	}
	zeitAusgabeEingebenUhrzeit(1);
}
void zeitEbeneE_EingebenDatum(){
	if(menueEinstellung == HIGH){
		encoderChanged = LOW;
		encoderPos = zeitJahr/100;

		zeitButtonStateB = 0;

		menueEinstellung = LOW;
	}

	if(encoderPos > 99) encoderPos = 20;
	if(encoderPos < 20) encoderPos = 20;

	if(encoderChanged == HIGH){
		zeitJahr += (encoderPos-(zeitJahr/100))*100;
		intZeitJahr = zeitJahr;
		zeitAusgabeEingebenDatum();

		encoderChanged = LOW;
	}
	zeitAusgabeEingebenUhrzeit(1);
}
void zeitEbeneF_EingebenDatum(){
	if(menueEinstellung == HIGH){
		encoderChanged = LOW;
		encoderPos = 0;
		menueAktion = 0;

		menueEinstellung = LOW;
	}

	if(encoderPos > 0){
		zeitJahr ++;
		encoderPos = 0;
	}
	else if(encoderPos < 0 && zeitJahr > 2000){
		zeitJahr --;
		encoderPos = 0;
	}
	else encoderPos = 0;


	if(encoderChanged == HIGH){
		intZeitJahr = zeitJahr;
		zeitAusgabeEingebenDatum();

		encoderChanged = LOW;
	}
	zeitAusgabeEingebenUhrzeit(1);

	if(encoderButtonPressed == LOW) zeitButtonStateB = 1;

	if(zeitButtonStateB == 1 && encoderButtonPressed == HIGH){
		zeitDatumEingebenFertig = HIGH;
		encoderPos = 0;
		menueEinstellung = HIGH;
		menueAdresse = 4211;
	}
}

/////////////////////////////////////////////////////

void zeitFuehrung(){
	switch(zeitQuelle){
	case 1:{	//Intern
		intZeitFuehrung();

		if(zeitUhrzeitEingebenFertig == HIGH){
			intZeitMinuten  = zeitMinuten;
			intZeitStunden  = zeitStunden;

			zeitUhrzeitEingebenFertig = LOW;
		}

		if(zeitDatumEingebenFertig == HIGH){
			intZeitMonat 	= zeitMonat;
			intZeitJahr 	= zeitJahr;

			intZeitSchaltjahrAenderung();

			if(zeitMonat == 2){
				if(zeitTag > intAnzahlTageFebruar) zeitTag = intAnzahlTageFebruar;
			}
			else if(zeitTag > anzahlTageMonate[zeitMonat-1]){
				zeitTag = anzahlTageMonate[zeitMonat-1];
			}

			intZeitTag 	 	= zeitTag;

			zeitSommerzeitTagCheck();

			zeitDatumEingebenFertig = LOW;
		}

		if(zeitUhrzeitEingebenStatus != HIGH){
			zeitSekunden = intZeitSekunden;
			zeitMinuten	 = intZeitMinuten;
			zeitStunden  = intZeitStunden;

			if(zeitSommerzeitZustand == 1 && menueAdresse != 32211){
				if(zeitStunden == 2 && zeitTag == zeitSommerzeitBeginnTag && zeitMonat == 3){
					intZeitStunden++;
				}
				if(zeitStunden == 3 && zeitTag == zeitSommerzeitEndTag && zeitMonat == 10 && zeitSommerzeitEndTagChange == HIGH){
					intZeitStunden--;
					zeitSommerzeitEndTagChange = LOW;
				}
			}
		}
		if(zeitDatumEingebenStatus != HIGH){
			zeitTag 	 = intZeitTag;
			zeitMonat 	 = intZeitMonat;
			zeitJahr 	 = intZeitJahr;
		}

		if(intSommerzeitTagCheckState == HIGH){
			zeitSommerzeitTagCheck();
			intSommerzeitTagCheckState = LOW;
		}
	}break;

	case 2:{	//GPS
		gpsFuehrung();

		gpsZeitzone = zeitZeitzone;

		zeitSekunden = GPS.seconds;
		zeitMinuten  = GPS.minute;
		zeitStunden  = gpsStunde;
		zeitTag 	 = gpsTag;
		zeitMonat 	 = gpsMonat;
		zeitJahr	 = gpsJahr;	//vierstellig

		if(GPS.fix){	//kein "Locaten"
			zeitDatumEingebenFertig = HIGH;
			zeitUhrzeitEingebenFertig = HIGH;
		}
	}break;
	}

	if(zeitDatumEingebenStatus != HIGH){
		// 1  = Sonntag, 2 = Montag, ... , 7 = Samstag
		// 0  = Datum ungültig z.B. 29.2.2007, 32.12.2007
		// -1 = Jahr kleiner als 1583 oder größer als 9999
		zeitWochentag = dayofweek(zeitTag, zeitMonat, zeitJahr);
	}
}


#endif















