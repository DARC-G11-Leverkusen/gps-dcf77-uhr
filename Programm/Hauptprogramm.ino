/*
 *	Dies ist das Hauptprogramm des Projektes "Arduino Uhr" der Jugendgruppe
 *	des OVs G11 Leverkusen von IGEL e.V. und DARC e.V. .
 */

#include <Arduino.h>
#include <LiquidCrystal.h>
#include "RotaryEncoder.h"
#include "MenueModul.h"
#include "IntegerInEEPROM.h"


///////////////////////////////////////////////////////////////////////////////////

void setup(){
	encoderBegin(19);
	menueBegin();
}

///////////////////////////////////////////////////////////////////////////////////

void loop(){
	switch(menueAdresse){
	//menueAdresse max. =2.147.483.647

	case 1:{	//(später) Ruhe-Anzeige-Ebene (Uhrzeit wird angezeigt usw.)
		//temporär://///////////////////////
		menueFuehrungZustand = HIGH;
		menueEintragSprung = LOW;
		menueRotaryEncoder = HIGH;
		menueZurueckPfeil = LOW;
		menueEintraegeAnzahl = 1;
		menueCursorZeichen[0] = "<";

		menueEintrag[1] = "Einstellungen";
		menueAktion[1] = 1;
		////////////////////////////////////
	}break;

	case 11:{	//erste Ebene der Einstellungen
		menueFuehrungZustand = HIGH;
		menueEintragSprung = LOW;
		menueRotaryEncoder = HIGH;
		menueZurueckPfeil = HIGH;
		menueEintraegeAnzahl = 1;
		menueCursorZeichen[0] = "<";

		menueEintrag[1] = "Wecker";
		menueAktion[1] = 1;
	}break;

	default:{	//Fehlerausgabe, wenn derzeitige Ebene nicht definiert ist
		menueFuehrungZustand = HIGH;
		menueEintragSprung = LOW;
		menueRotaryEncoder = HIGH;
		menueZurueckPfeil = HIGH;
		menueEintraegeAnzahl = 1;
		menueCursorZeichen[0] = "<";

		menueEintrag[1] = "*Ebene leer*";
		menueAktion[1] = 0;
	}break;
	}

	menueFuehrung();
}

///////////////////////////////////////////////////////////////////////////////////












