/*
 *	Dies ist ein Beispiel Programm für die Benutzung des Menü-Moduls des Projektes
 *	"Arduino Uhr" der Jugendgruppe des OVs G11 Leverkusen von IGEL e.V. und
 *	DARC e.V. .
 *
 *	geschrieben von Ralf Rumbler, DO3KV, 7.6.2018
 */

#include <Arduino.h>
#include "RotaryEncoder.h"	//Einbindung der Rotary Encoder Library
#include "MenueModul.h"		//Einbindung des Menü Moduls

///////////////////////////////////////////////////////////////////////////////////

void setup(){
	encoderBegin(2, 3, 19);	//"starten" der Rotary Encoder Library
	menueBegin();			//"starten" des Menü Moduls
}

///////////////////////////////////////////////////////////////////////////////////

void loop(){
	switch(menueAdresse){	//menueAdresse max. =2.147.483.647
	case 23:{	//Wenn die menueAdresse 23 ist, wird das folgende Menü erstellt:
		//////////Zu deklarierende Variablen://////////
		//Einstellungen:
		menueFuehrungZustand = HIGH;	//	Menü soll erstellt werden
		menueEintragSprung = LOW;		/*	kein Sprung bei weiterdrehen des Rotary
										 *  Encoders nach erreichen des letzten
										 *  Eintrages
										 */
		menueRotaryEncoder = HIGH;		//	Rotary Encoder soll Cursor steuern
		menueZurueckPfeil = HIGH;		/*	Ein Pfeil zum Springen in nächst höhere
										 *	Ebene soll vorhanden sein
										 */
		menueEintraegeAnzahl = 3;		//	Festlegung der Anzahl der Menü-Einträge
		menueCursorZeichen[0] = "<";	//	Auswahl des Cursor Zeichens

		//Einträge:
		menueEintrag[1] = "Eintrag A";	//	Zeichenfolge des Eintrages Nummer 1
		menueAktion[1] = 1;				/* 	Aktion bei Klicken auf Eintrag ist:
										 *	1 => in nächst tiefere Ebene springen
										 */
		menueEintrag[2] = "Eintrag B";	//	Zeichenfolge des Eintrages Nummer 2
		menueAktion[2] = -1;			/* 	Aktion bei Klicken auf Eintrag ist:
										 *	-1 => in nächst höhere Ebene springen
										 */
		menueEintrag[3] = "Eintrag C";	//	Zeichenfolge des Eintrages Nummer 3
										//	=> keine Aktion bei Klicken auf Eintrag
		///////////////////////////////////////////////
	}break;
	}

	menueFuehrung();	//Ausführung der Menü-Modul Funktionen
}












