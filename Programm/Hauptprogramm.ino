/*
 *	Dies ist das Hauptprogramm des Projektes "Arduino Uhr" der Jugendgruppe
 *	des OVs G11 Leverkusen von IGEL e.V. und DARC e.V. .
 */

#include <Arduino.h>
#include <LiquidCrystal.h>
#include "RotaryEncoder.h"
#include "IntegerInEEPROM.h"
#include "MenueModul.h"
#include "WeckerModul.h"

///////////////////////////////////////////////////////////////////////////////////

void setup(){
	encoderBegin(2, 3, 19);
	menueBegin();
}

///////////////////////////////////////////////////////////////////////////////////

void loop(){
	switch(menueAdresse){	//menueAdresse max. =2.147.483.647

	case 1:{	//(später) Ruhe-Anzeige-Ebene (Uhrzeit wird angezeigt usw.)
		//temporär://///////////////////////
		menueFuehrungZustand = HIGH;
		menueRotaryEncoder = LOW;
		menueZurueckPfeil = LOW;
		menueEintraegeAnzahl = 1;

		menueEintrag[1] = "*Ruhe-Ebene*";
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

		weckerButtonStateA = LOW;
	}break;

	case 111:{
		menueFuehrungZustand = HIGH;
		menueEintragSprung = LOW;
		menueRotaryEncoder = HIGH;
		menueZurueckPfeil = HIGH;
		menueEintraegeAnzahl = 5;
		menueCursorZeichen[0] = "<";

		menueEintrag[1] = "Wecker A";
		menueEintrag[2] = "Wecker B";
		menueEintrag[3] = "Wecker C";
		menueEintrag[4] = "Wecker D";
		menueEintrag[5] = "Wecker E";

		if(encoderButtonPressed == LOW){
			weckerButtonStateA = HIGH;
		}
		if(encoderButtonPressed == HIGH && weckerButtonStateA == HIGH){
			weckerAuswahl = menueAuswahl;
			menueReset();
			menueAdresse = 1111;
			Serial.println("X");
		}
	}break;

	case 1111:{
		menueFuehrungZustand = HIGH;
		menueEintragSprung = LOW;
		menueRotaryEncoder = HIGH;
		menueZurueckPfeil = HIGH;
		menueEintraegeAnzahl = 6;
		menueCursorZeichen[0] = "<";

		menueEintrag[1] = "An/Aus";
		menueAktion[1] = 0;
		menueEintrag[2] = "Zeit";
		menueAktion[2] = 0;
		menueEintrag[3] = "Wiederholen";
		menueAktion[3] = 0;
		menueEintrag[4] = "Weckerton";
		menueAktion[4] = 0;
		menueEintrag[5] = "Lautstaerke";
		menueAktion[5] = 0;
		menueEintrag[6] = "Entfernen";
		menueAktion[6] = 0;

		weckerButtonStateA = LOW;
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
	zurRuheEbene();
}

///////////////////////////////////////////////////////////////////////////////////

int zurRuheEbeneCounter = 0;
int zurRuheEbeneOldEncoderPos;

void zurRuheEbene(){
	if(menueAdresse != 1){
		if(encoderPos == zurRuheEbeneOldEncoderPos){
			if(zurRuheEbeneCounter < 8000){
				delay(5);
				zurRuheEbeneCounter += 5;
			}
			else{
				menueReset();
				menueAdresse = 1;
				encoderPos = 0;
				zurRuheEbeneCounter = 0;
			}
		}
		else{
			zurRuheEbeneCounter = 0;
			zurRuheEbeneOldEncoderPos = encoderPos;
		}
	}
	else{
		if(encoderPos != 0){
			menueReset();
			menueAdresse = 11;
			zurRuheEbeneOldEncoderPos = 0;
		}
	}
}









