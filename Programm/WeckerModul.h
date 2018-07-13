/*
 *	Wecker-Modul des Projektes "Arduino Uhr" der Jugendgruppe
 *	des OVs G11 Leverkusen von IGEL e.V. und DARC e.V. .
 *
 * 	geschrieben von Ralf Rumbler, DO3KV 13.7.2018
 */

#ifndef WeckerModul_h
#define WeckerModul_h

#include <Arduino.h>

int weckerDaten[5][6];
int weckerAuswahl;
byte weckerButtonStateA = LOW;

char weckerBufferA[12];
char weckerBufferB[12];
char weckerBufferC[12];
char weckerBufferD[12];

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
	if(weckerDaten[weckerAuswahl][3] == 0){
		menueEintrag[3][1] = " 1x";
	}
	else{
		menueEintrag[3][1] = "...";
	}
}
void weckerAusgabeTon(){
	memset(weckerBufferC, 0, sizeof weckerBufferC);

	menueEintrag[4][1] = "Nr. ";

	itoa(weckerDaten[weckerAuswahl][4], weckerBufferC, 10);
	menueEintrag[4][2] = weckerBufferC;
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

void weckerEbeneA(){
	if(menueEinstellung == HIGH){
		menueFuehrungZustand = HIGH;
		menueEintragSprung = LOW;
		menueRotaryEncoder = HIGH;
		menueZurueckPfeil = HIGH;
		menueEintraegeAnzahl = 5;
		menueCursorZeichen[0] = "<";

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
		menueFuehrungZustand = HIGH;
		menueEintragSprung = LOW;
		menueRotaryEncoder = HIGH;
		menueZurueckPfeil = HIGH;
		menueEintraegeAnzahl = 5;
		menueCursorZeichen[0] = "<";

		menueEintrag[1][0] = "Zustand:   ";
		menueAktion[1] = 2;
		menueEintrag[2][0] = "Zeit:    ";
		menueAktion[2] = 2;
		menueEintrag[3][0] = "Wiederh.:  ";
		menueAktion[3] = 0;
		menueEintrag[4][0] = "Ton:     ";
		menueAktion[4] = 0;
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
		menueCursorZeichen[0] = "X";
		menueAktion[1] = -1;

		encoderPos = 0;

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
void weckerEbeneC_ZeitA(){
	if(menueEinstellung == HIGH){
		menueRotaryEncoder = LOW;
		menueCursorZeichen[0] = "X";

		menueAktion[2] = 2;

		encoderPos = weckerDaten[weckerAuswahl][1];
		encoderChanged = LOW;

		menueEinstellung = LOW;
	}

	if(encoderPos > 12){
		encoderPos = 0;
	}
	if(encoderPos < 0){
		encoderPos = 12;
	}

	weckerDaten[weckerAuswahl][1] = encoderPos;

	if(encoderChanged == HIGH){
		weckerAusgabeZeit();
		encoderChanged = LOW;
	}
}
void weckerEbeneC_ZeitB(){
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
void weckerEbeneC_Lautstaerke(){
	if(menueEinstellung == HIGH){
		menueRotaryEncoder = LOW;
		menueCursorZeichen[0] = "X";

		menueAktion[5] = 2;

		encoderPos = weckerDaten[weckerAuswahl][5];
		encoderChanged = LOW;

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

#endif














