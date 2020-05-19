/*
 *	Dies ist das Helligkeit-Modul des Projektes "Arduino Uhr" der Jugendgruppe
 *	des OVs G11 Leverkusen von IGEL e.V. und DARC e.V. .
 *
 * 	geschrieben von Ralf Rumbler, DO3KV
 *
 *
 *	vom Modul benötigte EEPROM-Adressen: 39
 */

#ifndef HelligkeitModul_h
#define HelligkeitModul_h

#include <Arduino.h>

byte hellZustand;

///////////////////////////////////////////////////////////////////////////////////

void hellAusgabeZustand(){
	if(hellZustand == 1){
		strcpy_P(menueText, (const char*) F(" AN"));
		menueEintragZusatzErstellen(1, 12);
	}
	else{
		strcpy_P(menueText, (const char*) F("AUS"));
		menueEintragZusatzErstellen(1, 12);
	}
}

/////////////////////////////////////////////////////

void hellEbeneA(){
	if(menueEinstellung == HIGH){
		menueFuehrungZustand = HIGH;
		menueEintragSprung = LOW;
		menueRotaryEncoder = HIGH;
		menueHoeherPfeil = HIGH;
		menueCursorZeichen = '<';
		menueZeilenAnzahl = 1;

		hellAusgabeZustand();

		menueEinstellung = LOW;
	}

	strcpy_P(menueText, (const char*) F("Zustand:"));
	menueEintragErstellen(1, 2);

}
void hellEbeneB(){
	if(menueEinstellung == HIGH){
		menueRotaryEncoder = LOW;
		menueCursorZeichen = 'X';

		encoderChanged = LOW;
		encoderPos = hellZustand;

		menueEinstellung = LOW;
	}

	if(encoderPos > 1) encoderPos = 0;
	if(encoderPos < 0) encoderPos = 1;

	hellZustand = encoderPos;

	if(encoderChanged == HIGH){
		hellAusgabeZustand();
		encoderChanged = LOW;
	}
}
void hellEbeneC(){
	EEPROM.write(39, hellZustand);
	encoderPos = 0;
	menueAdresse = 311;
}

/////////////////////////////////////////////////////

void hellFuehrung(){

}

#endif




















