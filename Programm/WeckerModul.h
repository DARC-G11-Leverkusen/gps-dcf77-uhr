/*
 *	Wecker-Modul des Projektes "Arduino Uhr" der Jugendgruppe
 *	des OVs G11 Leverkusen von IGEL e.V. und DARC e.V. .
 *
 * 	geschrieben von Ralf Rumbler, DO3KV 25.10.2018
 *
 *
 *	vom Modul benötigte EEPROM-Adressen: 0 bis 34
 */

#ifndef WeckerModul_h
#define WeckerModul_h

#include <Arduino.h>
#include <EEPROM.h>

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
char* weckerTonName[3];
byte weckerEEPROMwriteState = LOW;
byte weckerWriteDaten;
byte weckerAnzahlToene;
/*	byte weckerStunde;	//temporär
 *	byte weckerMinute;	//temporär
 *	byte weckerTag;		//temporär
 */

char weckerBufferA[12];
char weckerBufferB[12];
char weckerBufferC[12];
char weckerBufferD[12];

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
		menueEintrag[1][1] = " AN";
	}
	else{
		menueEintrag[1][1] = "AUS";
	}
}
void weckerAusgabeZeit(){
	memset(weckerBufferA, 0, sizeof weckerBufferA);
	memset(weckerBufferB, 0, sizeof weckerBufferB);

	if(weckerDaten[weckerAuswahl][1] < 10){
		menueEintrag[2][1] = " ";
	}
	else{
		menueEintrag[2][1] = "";
	}
	itoa(weckerDaten[weckerAuswahl][1], weckerBufferA, 10);
	menueEintrag[2][2] = weckerBufferA;

	menueEintrag[2][3] = ":";

	if(weckerDaten[weckerAuswahl][2] < 10){
		menueEintrag[2][4] = "0";
	}
	else{
		menueEintrag[2][4] = "";
	}
	itoa(weckerDaten[weckerAuswahl][2], weckerBufferB, 10);
	menueEintrag[2][5] = weckerBufferB;
}
void weckerAusgabeWiederholen(){
	if(weckerDaten[weckerAuswahl][3] == 1){
		menueEintrag[3][1] = "...";
	}
	else{
		menueEintrag[3][1] = "AUS";
	}
}
void weckerAusgabeTon(){
	memset(weckerBufferC, 0, sizeof weckerBufferC);

	if(weckerDaten[weckerAuswahl][4] > 0){
		menueEintrag[4][1] = "Nr. ";

		itoa(weckerDaten[weckerAuswahl][4], weckerBufferC, 10);
		menueEintrag[4][2] = weckerBufferC;
	}
	else{
		menueEintrag[4][1] = "    -";
	}
}
void weckerAusgabeLautstaerke(){
	memset(weckerBufferD, 0, sizeof weckerBufferD);

	if(weckerDaten[weckerAuswahl][5] < 10){
		menueEintrag[5][1] = "  ";
	}
	else if(weckerDaten[weckerAuswahl][5] < 100){
		menueEintrag[5][1] = " ";
	}
	else{
		menueEintrag[5][1] = "";
	}

	itoa(weckerDaten[weckerAuswahl][5], weckerBufferD, 10);
	menueEintrag[5][2] = weckerBufferD;

	menueEintrag[5][3] = "%";
}

///////////////////////////////////

void weckerEbeneA(){
	if(menueEinstellung == HIGH){
		menueFuehrungZustand = HIGH;
		menueEintragSprung = LOW;
		menueRotaryEncoder = HIGH;
		menueZurueckPfeil = HIGH;
		menueZeilenAnzahl = 5;
		menueCursorZeichen = "<";

		menueEintrag[1][0] = "Wecker A";
		menueEintrag[2][0] = "Wecker B";
		menueEintrag[3][0] = "Wecker C";
		menueEintrag[4][0] = "Wecker D";
		menueEintrag[5][0] = "Wecker E";

		menueEinstellung = LOW;
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
		menueZurueckPfeil = HIGH;
		menueCursorZeichen = "<";

		menueZeilenAnzahl = 5;

		menueEintrag[1][0] = "Zustand:   ";
		menueAktion[1] = 2;
		menueEintrag[2][0] = "Zeit:    ";
		menueAktion[2] = 2;
		menueEintrag[3][0] = "Wiederh.:  ";
		menueAktion[3] = 1;
		menueEintrag[4][0] = "Ton:     ";
		menueAktion[4] = 1;
		menueEintrag[5][0] = "Lautst.:  ";
		menueAktion[5] = 2;

		weckerAusgabeZustand();
		weckerAusgabeZeit();
		weckerAusgabeWiederholen();
		weckerAusgabeTon();
		weckerAusgabeLautstaerke();

		menueEinstellung = LOW;
	}

	weckerButtonStateA = LOW;
}
void weckerEbeneC_Zustand(){
	if(menueEinstellung == HIGH){
		menueRotaryEncoder = LOW;
		menueCursorZeichen = "X";
		menueAktion[1] = -1;

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
		menueCursorZeichen = "X";

		menueAktion[2] = 2;

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
		menueAktion[2] = 2;

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
	if(menueEinstellung == HIGH){
		menueFuehrungZustand = HIGH;
		menueEintragSprung = LOW;
		menueRotaryEncoder = HIGH;
		menueZurueckPfeil = HIGH;
		menueCursorZeichen = "<";

		menueZeilenAnzahl = 7;

		menueEintrag[1][0] = "Mo";
		menueEintrag[2][0] = "Di";
		menueEintrag[3][0] = "Mi";
		menueEintrag[4][0] = "Do";
		menueEintrag[5][0] = "Fr";
		menueEintrag[6][0] = "Sa";
		menueEintrag[7][0] = "So";

		byte zaeler = 0;

		for(short i = 1; i <= 7; i++){
			menueEintrag[i][1] = " [";

			if(bitRead(weckerDaten[weckerAuswahl][6], i-1) == 0){
				menueEintrag[i][2] = " ";
			}
			else{
				menueEintrag[i][2] = "X";
			}

			zaeler += bitRead(weckerDaten[weckerAuswahl][6], i-1);

			if(zaeler > 0) weckerDaten[weckerAuswahl][3] = 1;
			else weckerDaten[weckerAuswahl][3] = 0;

			menueEintrag[i][3] = "]";
		}

		weckerEEPROMwriteState = HIGH;
		weckerWriteDaten = 3;

		menueEinstellung = LOW;
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

	weckerButtonStateA = LOW;

	menueEinstellung = HIGH;
	menueAdresse = 31111;
}
void weckerEbeneC_Ton(){
	if(menueEinstellung == HIGH){
		menueFuehrungZustand = HIGH;
		menueEintragSprung = LOW;
		menueRotaryEncoder = HIGH;
		menueZurueckPfeil = HIGH;
		menueCursorZeichen = "<";

		if(weckerAnzahlToene == 0){
			menueZeilenAnzahl = 1;

			menueEintrag[1][0] = "*leer*";
		}
		else{
			menueZeilenAnzahl = weckerAnzahlToene;
			for(byte i = 1; i <= weckerAnzahlToene; i++){
				menueEintrag[i][0] = weckerTonName[i-1];
				menueEintrag[i][1] = "[";
				menueEintrag[i][2] = " ";
				menueEintrag[i][3] = "]";
			}
			menueEintrag[weckerDaten[weckerAuswahl][4]][2] = "X";
		}

		weckerEEPROMwriteState = HIGH;
		weckerWriteDaten = 4;

		menueEinstellung = LOW;
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
		menueCursorZeichen = "X";

		menueAktion[5] = 2;

		encoderPos = weckerDaten[weckerAuswahl][5];
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

/////////////////////////////////////////////////////

/*void weckerFuehrung(){
	for(byte i = 0; i <= 4; i++){
		if(weckerDaten[i][2] == weckerMinute && weckerDaten[i][1] == weckerStunde && *Tag ist richtig* && weckerDaten[i][0] == HIGH){
			//wecker klingelt
		}
	}
}*/

#endif














