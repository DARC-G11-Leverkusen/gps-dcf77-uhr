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
		//Adresse max. =2147483647
		case   1:{
			menueFuehrungState = HIGH;		//	Menü an/aus (HIGH = an)
			 menueRotaryEncoderState = 1;	/*  1: Rotary Encoder wird von MenueFuehrung() benutzt (standart)
			 	 	 	 	 	 	 	 	 *  0: Rotary Encoder wird nicht von MenueFuehrung() benutzt (sondern z.B. für Auswahl von Infos)
			 	 	 	 	 	 	 	 	 */
			 menueEintragSprung = LOW;	   	/*	Sprung bei Ende des Menüs zum ersten Eintrag, wenn heruntergedreht wird
			 						   	 	 *	UND Sprung bei Anfang des Menüs zum letzten Eintrag, wenn heraufgedreht wird:
			 						   	 	 *	an/aus (HIGH = an)
			 						   	 	 */
			 menueCursorZeichen[0] = "<";	//	Cursorzeichen (bei Auswahl von Infos z.B. "X")
			 menueEintraegeAnzahl = 3;  	//	Anzahl der Menüeinträge

			 menueEintrag[1] = "AA"; 		//  Inhalt vom Menüeintrag
			 menueAktion[1] = 0;			/*	Aktion bei drücken Auf Menüeintrag ist...
			 	 	 	 	 	 	 	 	 *	-1 = in nächst höhere Ebene/Adresse gehen
			 	 	 	 	 	 	 	 	 *	  => !! ein "Zurück-Pfeil" ist bei MenueFuehrung() schon implementiert !!
			 	 	 	 	 	 	 	 	 *	 0 = (noch) keine Aktion
			 	 	 	 	 	 	 	 	 *	 1 = in nächst tiefere Ebene/Adresse gehen
			 	 	 	 	 	 	 	 	 */
			 menueEintrag[2] = "BBB";
			 menueAktion[2] = 1;
			 menueEintrag[3] = "C";
			 menueAktion[3] = 1;
		}break;

		case   21:{
			menueFuehrungState = HIGH;
			 menueEintragSprung = HIGH;
			 menueEintraegeAnzahl = 4;

			 menueEintrag[1] = "Q1";
			 menueAktion[1] = 0;
			 menueEintrag[2] = "ZPO";
			 menueAktion[2] = 0;
			 menueEintrag[3] = "6";
			 menueAktion[3] = 0;
			 menueEintrag[4] = "90";
			 menueAktion[4] = 1;
		}break;

		default:{
			menueFuehrungState = HIGH;
			 menueEintraegeAnzahl = 1;

			 menueEintrag[1] = "*Ebene leer*";
			 menueAktion[1] = 0;
		}break;
	}

	menueFuehrung();
}

///////////////////////////////////////////////////////////////////////////////////












