/*
 *	Dies ist das interne Zeitberechnungsmodul des Projektes "Arduino Uhr" der Jugendgruppe
 *	des OVs G11 Leverkusen von IGEL e.V. und DARC e.V. .
 *
 * 	geschrieben von Ralf Rumbler, DO3KV
 */

#ifndef InterneZeitModul_h
#define InterneZeitModul_h

#include <Arduino.h>

#include "BrueckenModul.h"

byte intZeitSekunden, intZeitMinuten, intZeitStunden, intZeitTag = 1, intZeitMonat = 1;
int intZeitJahr = 2000;
byte intAnzahlTageFebruar = 28;
byte intZeitZaehler;
byte intSommerzeitTagCheckState;
byte intHalbeSekunde = 0;

///////////////////////////////////////////////////////////////////////////////////

void intZeitBegin(){
	// Timer 1
	noInterrupts();           // Alle Interrupts temporär abschalten
	TCCR1A = 0;
	TCCR1B = 0;

	TCNT1 = 34286;            // Timer nach obiger Rechnung vorbelegen
	TCCR1B |= (1 << CS12);    // 256 als Prescale-Wert spezifizieren
	TIMSK1 |= (1 << TOIE1);   // Timer Overflow Interrupt aktivieren
	interrupts();             // alle Interrupts scharf schalten

	intHalbeSekunde = 0;
}

// Hier kommt die selbstdefinierte Interruptbehandlungsroutine
// f�r den Timer Overflow
ISR(TIMER1_OVF_vect)
{
  TCNT1 = 34286;             // Zähler erneut vorbelegen
  if(zeitQuelle == 1) intZeitZaehler++;
  intHalbeSekunde = intHalbeSekunde ^ 1;
}


/////////////////////////////////////////////////////

void intZeitSchaltjahrAenderung(){
	if(intZeitJahr % 4 == 0){
		if(intZeitJahr % 100 == 0){
			intAnzahlTageFebruar = 28;
		}
		if(intZeitJahr % 100 == 0 && intZeitJahr % 400 == 0){
			intAnzahlTageFebruar = 29;
		}
		else{
			intAnzahlTageFebruar = 29;
		}
	}
	else{
		intAnzahlTageFebruar = 28;
	}
}

void intZeitFuehrung(){
	if(intZeitZaehler == 2){
		intZeitSekunden++;
		intZeitZaehler = 0;
	}
	if(intZeitSekunden == 60){
		intZeitMinuten++;
		intZeitSekunden = 0;
	}
	if(intZeitMinuten == 60){
		intZeitStunden++;
		intZeitMinuten = 0;
	}
	if(intZeitStunden == 24){
		intZeitTag++;
		intZeitStunden = 0;
	}

	if(intZeitMonat == 2){
		if(intZeitTag > (intAnzahlTageFebruar)){
			intZeitMonat++;
			intZeitTag = 1;
		}
	}
	else if(intZeitTag > (anzahlTageMonate[intZeitMonat-1])){
		intZeitMonat++;
		intZeitTag = 1;
	}

	if(intZeitMonat == 13){
		intZeitJahr++;
		intZeitMonat = 1;

		intZeitSchaltjahrAenderung();
		intSommerzeitTagCheckState = HIGH;
	}
}


#endif















