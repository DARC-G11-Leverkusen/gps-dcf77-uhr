/*
 *	Dies ist das Hauptprogramm des Projektes "Arduino Uhr" der Jugendgruppe
 *	des OVs G11 Leverkusen von IGEL e.V. und DARC e.V. .
 */

/***BELEGTE EEPROM-ADRESSEN***
 *
 * 	Wecker-Modul:  0 bis 34
 * 	Zeit-Modul:   35 bis 37
 *
 */

#include <Arduino.h>
#include <LiquidCrystal.h>
#include "RotaryEncoder.h"
#include "IntegerInEEPROM.h"
#include "MenueModul.h"
#include "WeckerModul.h"
#include "ZeitModul.h"

///////////////////////////////////////////////////////////////////////////////////

void setup(){
	encoderBegin(2, 3, 19);
	menueBegin();

	weckerEEPROMread();
	zeitEEPROMread();
}

///////////////////////////////////////////////////////////////////////////////////

void loop(){
	switch(menueAdresse){	//menueAdresse max. =2.147.483.647

	case 1:{	//(später) Ruhe-Anzeige-Ebene (Uhrzeit wird angezeigt usw.)
		//temporär://///////////////////////
		if(menueEinstellung == HIGH){
			menueFuehrungZustand = HIGH;
			menueRotaryEncoder = LOW;
			menueZurueckPfeil = LOW;
			menueZeilenAnzahl = 1;

			menueEintrag[1][1] = "*Ruhe-Ebene*";
			menueAktion[1] = 1;

			menueEinstellung = LOW;
		}
		////////////////////////////////////
	}break;

	case 11:{	//erste Ebene der Einstellungen
		if(menueEinstellung == HIGH){
			menueFuehrungZustand = HIGH;
			menueEintragSprung = LOW;
			menueRotaryEncoder = HIGH;
			menueZurueckPfeil = HIGH;
			menueZeilenAnzahl = 2;
			menueCursorZeichen = "<";

			menueEintrag[1][1] = "Wecker";
			menueAktion[1] = 1;
			menueEintrag[2][1] = "Zeit";
			menueAktion[2] = 1;

			weckerButtonStateA = LOW;

			menueEinstellung = LOW;
		}
	}break;

	//Wecker-Modul//////////////////////////////////////////////////
	case 111: weckerEbeneA(); break;
	 case 1111: weckerEbeneB(); break;
	  case 11111: weckerEbeneC_Zustand(); break;
	  case 21111: weckerEbeneC_Zeit(); break;
	   case 221111: weckerEbeneD_Zeit(); break;
	    case 2221111: encoderPos = 0; menueAdresse = 1111; break;
	  case 31111: weckerEbeneC_Wiederholen(); break;
	   case 131111: weckerEbeneD_Wiederholen(); break;
      case 41111: weckerEbeneC_Ton(); break;
	   case 141111: weckerEbeneD_Ton(); break;
	  case 51111: weckerEbeneC_Lautstaerke(); break;
	   case 551111: encoderPos = 0; menueAdresse = 1111; break;
	////////////////////////////////////////////////////////////////

	//Zeit-Modul////////////////////////////////////////////////////
	case 211: zeitEbeneA(); break;
	 case 1211: zeitEbeneB_Zeitzone(); break;
	  case 11211: zeitEbeneC_Zeitzone(); break;
	 case 2211: zeitEbeneB_Quelle(); break;
	  case 22211: zeitEbeneC_Quelle(); break;
	 case 3211: zeitEbeneB_Eingeben(); break;
	  case 13211: zeitEbeneC_EingebenUhrzeit(); break;
	   case 113211: zeitEbeneD_EingebenUhrzeit(); break;
	    case 1113211: encoderPos = 0; menueAdresse = 3211; break;
	  case 23211: zeitEbeneC_EingebenDatum(); break;
	   case 223211: zeitEbeneD_EingebenDatum(); break;
	    case 2223211: zeitEbeneE_EingebenDatum(); break;
	     case 22223211: encoderPos = 0; menueAdresse = 3211; break;
	////////////////////////////////////////////////////////////////

	default:{	//Fehlerausgabe, wenn derzeitige Ebene nicht definiert ist
		if(menueEinstellung == HIGH){
			menueReset();

			menueFuehrungZustand = HIGH;
			menueEintragSprung = LOW;
			menueRotaryEncoder = HIGH;
			menueZurueckPfeil = HIGH;
			menueZeilenAnzahl = 1;
			menueCursorZeichen = "<";

			menueEintrag[1][1] = "*Ebene leer*";
			menueAktion[1] = 0;

			menueEinstellung = LOW;
		}
	}break;
	}

	menueFuehrung();
	zeitFuehrung();
	zurRuheEbene();
}

///////////////////////////////////////////////////////////////////////////////////

int zurRuheEbeneCounter = 0;
int zurRuheEbeneOldEncoderPos;

void zurRuheEbene(){
	if(menueAdresse != 1){
		if(encoderPos == zurRuheEbeneOldEncoderPos){
			if(zurRuheEbeneCounter < 12000){
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









