#ifndef GpsModul_h
#define GpsModul_h

#include "Arduino.h"
#include "SmallAdafruit_GPS.h"
#include "UebergreifendeProgramme.h"

SoftwareSerial gpsSerial(A3, A2); // TX, RX
SmallAdafruit_GPS GPS(&gpsSerial);

#define GPSECHO  false
#define DSTPIN A1
#define ROUND_US(d) ( (int) ((d) + ((d) > 0 ? 0.5 : -0.5)) )


//////////////////////////////////////////////////////////////////////////////////


boolean gpsUsingInterrupt = false;
void gpsUseInterrupt(boolean);
int gpsZeitzone;
byte gpsStunde, gpsTag, gpsMonat;
int gpsJahr;


//////////////////////////////////////////////////////////////////////////////////


void gpsBegin(){
	Serial.begin(115200);
	GPS.begin(9600);

	GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
	GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
	GPS.sendCommand(PGCMD_ANTENNA);

	gpsUseInterrupt(true);
}

SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
#ifdef UDR0
  if (GPSECHO)
    if (c) UDR0 = c;
#endif
}

void gpsUseInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    gpsUsingInterrupt = true;
  }
  else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    gpsUsingInterrupt = false;
  }
}

void gpsFuehrung(){
	if (! gpsUsingInterrupt) {
		char c = GPS.read();
		if (GPSECHO)
			if (c) Serial.print(c);
	}

	if (GPS.newNMEAreceived()) {
		if (!GPS.parse(GPS.lastNMEA()))
			return;
	}

	////////////////////////////Korrekturen der rohen Werte////////////////////////////
	gpsJahr   = GPS.year+2000;
	gpsMonat  = GPS.month;
	gpsTag 	  = GPS.day;

	if(gpsZeitzone < 0){	//wenn Stunde geändert werden muss
		if(GPS.hour < (gpsZeitzone*-1)){	//wenn Tag auch geändert werden muss
			if(GPS.day > 1) gpsTag = GPS.day-1;
			else{	//wenn Monat auch geändert werden muss
				if(GPS.month > 1){
					gpsMonat = GPS.month-1;

					if(gpsMonat != 2) gpsTag = anzahlTageMonate[gpsMonat-1];
					else 			  gpsTag = anzahlTageFebruar(gpsJahr);	  //wenn Februar ist
				}
				else{	//wenn Jahr auch geändert werden muss
					gpsJahr  = GPS.year+1999;
					gpsMonat = 12;
					gpsTag	 = 31;
				}
			}

			gpsStunde = GPS.hour+gpsZeitzone+24;
		}
		else gpsStunde = GPS.hour+gpsZeitzone;
	}
	else if(gpsZeitzone > 0){	//wenn Stunde geändert werden muss
		if(GPS.hour <= 23 && GPS.hour >= (24-gpsZeitzone)){	//wenn Tag auch geändert werden muss
			if(GPS.month == 2 && GPS.day < anzahlTageFebruar(gpsJahr)){
				gpsTag = GPS.day+1;
			}
			else if(GPS.day < anzahlTageMonate[gpsMonat-1]){
				gpsTag = GPS.day+1;
			}
			else{	//wenn Monat auch geändert werden muss
				if(GPS.month == 12){		//wenn Jahr auch geändert werden muss
					gpsJahr  = GPS.year+2001;
					gpsMonat = 1;
				}
				else{
					gpsMonat = GPS.month+1;
				}

				gpsTag = 1;
			}
		}

		if((GPS.hour+gpsZeitzone) < 24) gpsStunde = GPS.hour+gpsZeitzone;
		else							gpsStunde = GPS.hour+gpsZeitzone-24;
	}
	else{
		gpsStunde = GPS.hour;
	}
	///////////////////////////////////////////////////////////////////////////////////


}

#endif

















