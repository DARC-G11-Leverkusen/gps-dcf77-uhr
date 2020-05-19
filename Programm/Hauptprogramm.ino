/*
 *	Dies ist das Hauptprogramm des Projektes "Arduino Uhr" der Jugendgruppe
 *	des OVs G11 Leverkusen von IGEL e.V. und DARC e.V. .
 */

/***BELEGTE EEPROM-ADRESSEN***
 *
 * 	Wecker-Modul:  	    0 bis 34
 * 	Zeit-Modul:   	   35 bis 38
 * 	Helligkeit-Modul:  39
 */

#include <Arduino.h>
#include "RotaryEncoder.h"
#include "IntegerInEEPROM.h"
#include "MenueModul.h"
#include "InterneZeitModul.h"
#include "WeckerModul.h"
#include "ZeitModul.h"
#include "HelligkeitModul.h"

///////////////////////////////////////////////////////////////////////////////////

void setup(){
	intZeitBegin();
	encoderBegin(2, 3, 19);
	lcd.begin(16, 2);
	Serial.begin(115200); // connect at 115200 so we can read the GPS fast enough and echo without dropping chars // also spit it out
	gpsBegin();
	intZeitSchaltjahrAenderung();
	zeitSommerzeitTagCheck();

	weckerEEPROMread();
	zeitEEPROMread();
	hellZustand = EEPROM.read(39);
}

///////////////////////////////////////////////////////////////////////////////////

void loop(){
	switch(menueAdresse){	//menueAdresse max. =4.294.967.295

	case 1:{	//Ruhe-Anzeige-Ebene (Uhrzeit wird angezeigt usw.)
		if(menueEinstellung == HIGH){
			menueFuehrungZustand = HIGH;
			menueRotaryEncoder = LOW;
			menueHoeherPfeil = LOW;
			menueZeilenAnzahl = 2;

			menueAktion = 1;

			zeitUhrzeitEingebenStatus = LOW;
			zeitDatumEingebenStatus   = LOW;

			menueEinstellung = LOW;
		}

		if(zeitQuelle == 2 || zeitQuelle == 1){
			ausgabeUhrzeit(zeitStunden, zeitMinuten, 1, 1, 0);

			if(zeitQuelle == 2){
				lcd.setCursor(15,1);

				if(!GPS.fix) lcd.print(F("L"));
				else		 lcd.print(F(" "));
			}

			lcd.setCursor(0,1);
			wochentagPrint(1);
			lcd.print(zeitTag);
			lcd.print(F("."));
			lcd.print(zeitMonat);
			lcd.print(F("."));
			lcd.print(zeitJahr);
			if(zeitTag <= 9 && zeitMonat <= 9) lcd.print(F("  "));
			else if(zeitTag <= 9 || zeitMonat <= 9) lcd.print(F(" "));
		}
		else{
			lcd.setCursor(0,0);
			lcd.print(F("*Error*"));
		}

	}break;

	case 11:{	//erste Ebene der Einstellungen
		if(menueEinstellung == HIGH){
			menueFuehrungZustand = HIGH;
			menueEintragSprung = LOW;
			menueRotaryEncoder = HIGH;
			menueHoeherPfeil = HIGH;
			menueZeilenAnzahl = 3;
			menueCursorZeichen = '<';

			weckerButtonStateA = LOW;

			menueEinstellung = LOW;
		}

		if(menueCursorBewegt == HIGH){
			strcpy_P(menueText, (const char*) F("Wecker"));
			menueEintragErstellen(1, 1);
			strcpy_P(menueText, (const char*) F("Zeit"));
			menueEintragErstellen(2, 1);
			strcpy_P(menueText, (const char*) F("Helligkeit"));
			menueEintragErstellen(3, 1);

			strcpy_P(menueText, (const char*) F("=>"));
			for(byte i = 1; i<=3; i++){
				menueEintragZusatzErstellen(i, 13);
			}

			menueCursorBewegt = LOW;
		}
	}break;

	//Wecker-Modul//////////////////////////////////////////////////
	case 2: weckerKlingelnA(); break;
	 case 12: menueReset(); menueAdresse = 1; break;
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
	   case 1211: zeitEbeneB_Quelle(); break;
	    case 11211: zeitEbeneC_Quelle(); break;
	   case 2211: zeitEbeneB_Sommerzeit(); break;
	    case 22211: zeitEbeneC_Sommerzeit(); break;
	   case 3211: zeitEbeneB_Weiterleitung(); break;
	    case 33211: zeitEbeneC_Zeitzone(); break;
	     case 333211: zeitEbeneD_Zeitzone(); break;
	   case 4211: zeitEbeneB_Eingeben(); break;
	    case 14211: zeitEbeneC_EingebenUhrzeit(); break;
	     case 114211: zeitEbeneD_EingebenUhrzeit(); break;
		  case 1114211: zeitEbeneE_EingebenUhrzeit(); break;
	    case 24211: zeitEbeneC_EingebenDatum(); break;
	     case 224211: zeitEbeneD_EingebenDatum(); break;
	      case 2224211: zeitEbeneE_EingebenDatum(); break;
	       case 22224211: zeitEbeneF_EingebenDatum(); break;
	////////////////////////////////////////////////////////////////

	//Helligkeit-Modul//////////////////////////////////////////////
	  case 311: hellEbeneA(); break;
	   case 1311: hellEbeneB(); break;
	    case 11311: hellEbeneC(); break;
	////////////////////////////////////////////////////////////////


	default:{	//Fehlerausgabe, wenn derzeitige Ebene nicht definiert ist
		if(menueEinstellung == HIGH){
			menueReset();

			menueFuehrungZustand = HIGH;
			menueEintragSprung = LOW;
			menueRotaryEncoder = HIGH;
			menueHoeherPfeil = HIGH;
			menueZeilenAnzahl = 1;
			menueCursorZeichen = '<';

			strcpy_P(menueText, (const char*) F("*Ebene leer*"));
			menueEintragErstellen(1, 0);

			menueEinstellung = LOW;
		}
	}break;
	}

	hellFuehrung();
	menueFuehrung();
	zeitFuehrung();
	weckerFuehrung();
	zurRuheEbene();

	//Serial.println(menueZeilenAnzahl);
}

///////////////////////////////////////////////////////////////////////////////////

int zurRuheEbeneCounter = 0;
int zurRuheEbeneOldEncoderPos;

void zurRuheEbene(){
	if(menueAdresse != 1 && menueAdresse != 2){
		if(encoderPos == zurRuheEbeneOldEncoderPos){
			if(zurRuheEbeneCounter < 4000){
				delay(1);
				//zurRuheEbeneCounter += 1;
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
	else if(menueAdresse != 2){
		if(encoderPos != 0){
			menueReset();
			menueAdresse = 11;
			zurRuheEbeneOldEncoderPos = 0;
		}
	}
}

void wochentagPrint(byte komma){
	switch(zeitWochentag){
	case 1:{ lcd.print(F("So")); }break;

	case 2:{ lcd.print(F("Mo")); }break;

	case 3:{ lcd.print(F("Di")); }break;

	case 4:{ lcd.print(F("Mi")); }break;

	case 5:{ lcd.print(F("Do")); }break;

	case 6:{ lcd.print(F("Fr")); }break;

	case 7:{ lcd.print(F("Sa")); }break;

	default:{ lcd.print(F(" ?")); }break;
	}

	if(komma == HIGH) lcd.print(F(", "));
}










