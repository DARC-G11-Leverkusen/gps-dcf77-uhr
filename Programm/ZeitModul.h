/*
 *	Dies ist das Zeit-Modul des Projektes "Arduino Uhr" der Jugendgruppe
 *	des OVs G11 Leverkusen von IGEL e.V. und DARC e.V. .
 *
 * 	geschrieben von Ralf Rumbler, DO3KV
 *
 *
 *	vom Modul benötigte EEPROM-Adressen: 35 bis 37
 */

#ifndef ZeitModul_h
#define ZeitModul_h

#include <Arduino.h>
#include "uebergreifendeProgramme.h"

int zeitZeitzone;
byte zeitQuelle;
byte zeitStunden, zeitMinuten, zeitTag = 1, zeitMonat = 1;
int zeitJahr;

///////////////////////////////////////////////////////////////////////////////////

void zeitEEPROMread(){
	zeitQuelle = EEPROM.read(35);
	zeitZeitzone = eepromReadInt(36);
}

/////////////////////////////////////////////////////

void zeitAusgabeZeitzone(){
	if(zeitZeitzone == 0) menueEintrag[1][2] = "  ";
	else if(zeitZeitzone >= 1 && zeitZeitzone <= 9) menueEintrag[1][2] = " +";
	else if(zeitZeitzone >= 10) menueEintrag[1][2] = "+";
	else if(zeitZeitzone <= -1 && zeitZeitzone >= -9) menueEintrag[1][2] = " ";
	else if(zeitZeitzone <= -10) menueEintrag[1][2] = "";
	itoa(zeitZeitzone, itoaBufferA, 10);
	menueEintrag[1][3] = itoaBufferA;
}
void zeitAusgabeQuelle(){
	switch(zeitQuelle){
	case 0:{
		menueEintrag[2][2] = "  Auto";
	}break;
	case 1:{
		menueEintrag[2][2] = "Intern";
	}break;
	case 2:{
		menueEintrag[2][2] = "   GPS";
	}break;
	case 3:{
		menueEintrag[2][2] = " DCF77";
	}break;
	}
}
void zeitAusgabeEingebenUhrzeit(){
	itoa(zeitStunden, itoaBufferA, 10);
	itoa(zeitMinuten, itoaBufferB, 10);

	if(zeitStunden < 10) menueEintrag[1][1] = " ";
	else menueEintrag[1][1] = "";
	menueEintrag[1][2] = itoaBufferA;
	menueEintrag[1][3] = ":";
	if(zeitMinuten < 10) menueEintrag[1][4] = "0";
	else menueEintrag[1][4] = "";
	menueEintrag[1][5] = itoaBufferB;
}
void zeitAusgabeEingebenDatum(){
	if(zeitTag < 1) zeitTag = 1;
	if(zeitMonat < 1) zeitMonat = 1;

	itoa(zeitTag, itoaBufferC, 10);
	itoa(zeitMonat, itoaBufferD, 10);
	itoa(zeitJahr, itoaBufferE, 10);

	if(zeitTag <= 9 && zeitMonat <= 9){
		if(zeitJahr <= 9){
			menueEintrag[2][1] = "   ";
		}
		else if(zeitJahr <= 99){
			menueEintrag[2][1] = "  ";
		}
	}

	else if(zeitTag <= 9 && zeitMonat >= 10){
		if(zeitJahr <= 9){
			menueEintrag[2][1] = "  ";
		}
		else if(zeitJahr <= 99){
			menueEintrag[2][1] = " ";
		}
	}

	else if(zeitTag >= 10 && zeitMonat <= 9){
		if(zeitJahr <= 9){
			menueEintrag[2][1] = "  ";
		}
		else if(zeitJahr <= 99){
			menueEintrag[2][1] = " ";
		}
	}

	else{
		if(zeitJahr <= 9){
			menueEintrag[2][1] = " ";
		}
		else if(zeitJahr <= 99){
			menueEintrag[2][1] = "";
		}
	}

	menueEintrag[2][2] = itoaBufferC;
	menueEintrag[2][3] = ".";
	menueEintrag[2][4] = itoaBufferD;
	menueEintrag[2][5] = ".";
	menueEintrag[2][6] = itoaBufferE;
}

///////////////////////////////////

void zeitEbeneA(){
	if(menueEinstellung == HIGH){
		menueFuehrungZustand = HIGH;
		menueEintragSprung = LOW;
		menueRotaryEncoder = HIGH;
		menueZurueckPfeil = HIGH;
		menueZeilenAnzahl = 4;
		menueCursorZeichen = "<";

		menueEintrag[1][1] = "Zeitzone:  ";
		menueAktion[1] = 2;
		menueEintrag[2][1] = "Quelle: ";
		menueAktion[2] = 2;
		menueEintrag[3][0] = "Eingeben ";
		menueAktion[3] = 1;
		menueEintrag[4][1] = "Sommer/Winter";
		menueAktion[4] = 0;

		zeitAusgabeZeitzone();
		zeitAusgabeQuelle();

		menueEinstellung = LOW;
	}
}
void zeitEbeneB_Zeitzone(){
	if(menueEinstellung == HIGH){
		menueRotaryEncoder = LOW;
		menueCursorZeichen = "X";
		menueAktion[1] = 2;

		encoderChanged = LOW;
		encoderPos = zeitZeitzone;

		menueEinstellung = LOW;
	}

	if(encoderPos < -12) encoderPos = 12;
	if(encoderPos > 12) encoderPos = -12;

	zeitZeitzone = encoderPos;

	if(encoderChanged == HIGH){
		zeitAusgabeZeitzone();
		encoderChanged = LOW;
	}
}
void zeitEbeneC_Zeitzone(){
	eepromWriteInt(36, zeitZeitzone);
	menueReset();
	menueAdresse = 211;
}
void zeitEbeneB_Quelle(){
	if(menueEinstellung == HIGH){
		menueRotaryEncoder = LOW;
		menueCursorZeichen = "X";
		menueAktion[2] = 2;

		encoderChanged = LOW;
		encoderPos = zeitQuelle;

		menueEinstellung = LOW;
	}
	if(encoderPos > 3) encoderPos = 0;
	if(encoderPos < 0) encoderPos = 3;

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
void zeitEbeneB_Eingeben(){
	if(menueEinstellung == HIGH){
		menueFuehrungZustand = HIGH;
		menueEintragSprung = LOW;
		menueRotaryEncoder = HIGH;
		menueZurueckPfeil = HIGH;
		menueZeilenAnzahl = 2;
		menueCursorZeichen = "<";

		menueEintrag[1][0] = "Uhrzeit: ";
		menueAktion[1] = 2;
		menueEintrag[2][0] = "Dat.: ";
		menueAktion[2] = 2;

		zeitAusgabeEingebenUhrzeit();
		zeitAusgabeEingebenDatum();

		menueEinstellung = LOW;
	}
}
void zeitEbeneC_EingebenUhrzeit(){
	if(menueEinstellung == HIGH){
		menueRotaryEncoder = LOW;
		menueCursorZeichen = "X";

		encoderChanged = LOW;
		encoderPos = zeitStunden;

		menueEinstellung = LOW;
	}

	if(encoderPos > 23) encoderPos = 0;
	if(encoderPos < 0) encoderPos = 23;

	zeitStunden = encoderPos;

	if(encoderChanged == HIGH){
		zeitAusgabeEingebenUhrzeit();
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
		zeitAusgabeEingebenUhrzeit();
		encoderChanged = LOW;
	}
}
void zeitEbeneC_EingebenDatum(){	//!!!! maximale Höhe von zeitTag ist nicht immer =31 !!!!
	if(menueEinstellung == HIGH){
		menueRotaryEncoder = LOW;
		menueCursorZeichen = "X";

		encoderChanged = LOW;
		encoderPos = zeitTag;

		menueEinstellung = LOW;
	}

	if(encoderPos > 31) encoderPos = 1; //!!!! maximale Höhe von zeitTag ist nicht immer =31 !!!!
	if(encoderPos < 1) encoderPos = 31;	//!!!! maximale Höhe von zeitTag ist nicht immer =31 !!!!

	zeitTag = encoderPos;

	if(encoderChanged == HIGH){
		zeitAusgabeEingebenDatum();
		encoderChanged = LOW;
	}
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
}
void zeitEbeneE_EingebenDatum(){
	if(menueEinstellung == HIGH){
		encoderChanged = LOW;
		encoderPos = zeitJahr;

		menueEinstellung = LOW;
	}

	if(encoderPos > 99) encoderPos = 0;
	if(encoderPos < 0) encoderPos = 99;

	zeitJahr = encoderPos;

	if(encoderChanged == HIGH){
		zeitAusgabeEingebenDatum();
		encoderChanged = LOW;
	}
}

/////////////////////////////////////////////////////

void zeitFuehrung(){
	/*if(zeitQuelle == 2){
		gpsOffset = zeitZeitzone;

		zeitStunden = gpsHour;
		zeitMinuten = gpsMinute;
		zeitTag 	= gpsDay;
		zeitMonat 	= gpsMonth;
		zeitJahr	= gpsYear;
	}*/
}

#endif















