/*
 *	In dieser Datei befinden sich Programme, auf welche mehrere Module des
 *	Projektes zugreifen.
 *
 *	Es ist Teil des Projektes "Arduino Uhr" der Jugendgruppe des OVs G11 Leverkusen
 *	von IGEL e.V. und DARC e.V. .
 *
 *	Wochentagsberechnung von Franz Bachler (http://members.aon.at/fraba/wochtag.htm)
 */

#ifndef BrueckenModul_h
#define BrueckenModul_h

char itoaBufferA[12];
char itoaBufferB[12];
char itoaBufferC[12];
char itoaBufferD[12];

char itoaIntBufferA[16];

byte zeitQuelle;
const byte anzahlTageMonate[]={31,28,31,30,31,30,31,31,30,31,30,31};

byte anzahlTageFebruar(int Jahr){
	if(Jahr % 4 == 0){
		if(Jahr % 100 == 0){
			return(28);
		}
		if(Jahr % 100 == 0 && Jahr % 400 == 0){
			return(29);
		}
		else{
			return(29);
		}
	}
	else{
		return(28);
	}
}

void ausgabeUhrzeit(byte Stunde, byte Minute, byte Zeile, byte Spalte, byte DoppelpunktState){
	//memset(itoaBufferA, 0, sizeof itoaBufferA);
	//memset(itoaBufferB, 0, sizeof itoaBufferB);

	itoa(Stunde, itoaBufferA, 10);
	strcpy(menueText, itoaBufferA);

	if(Stunde < 10){
		menueEintragZusatzErstellen(Zeile, Spalte+1);

		strcpy_P(menueText, (const char*) F(" "));
		menueEintragZusatzErstellen(Zeile, Spalte);
	}
	else{
		menueEintragZusatzErstellen(Zeile, Spalte);
	}

	if(DoppelpunktState == 0){
		strcpy_P(menueText, (const char*) F(":"));
		menueEintragZusatzErstellen(Zeile, Spalte+2);
	}

	itoa(Minute, itoaBufferB, 10);
	strcpy(menueText, itoaBufferB);

	if(Minute < 10){
		menueEintragZusatzErstellen(Zeile, Spalte+4);
		strcpy_P(menueText, (const char*) F("0"));
		menueEintragZusatzErstellen(Zeile, Spalte+3);
	}
	else{
		menueEintragZusatzErstellen(Zeile, Spalte+3);
	}
}

//Wochentagsberechnung/////////////////////////////////

//von Franz Bachler

// gibt Wochentag zur¸ck
// 1  = Sonntag, 2 = Montag, ... , 7 = Samstag
// 0  = Datum ung¸ltig z.B. 29.2.2007, 32.12.2007
// -1 = Jahr kleiner als 1583 oder grˆﬂer als 9999

int dayofweek(int d, int m, int y)                  // Tag, Monat, Jahr; Jahr unbedingt vierstellig
{
  int s=0, mtag[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};    // Anzahl der Tage pro Monat
  if ((y % 4==0 && y % 100!=0) || y % 400==0) s=1;              // Schaltjahrescheck
  if (y<1583 || y>9999) return(-1);                             // Jahr auf sinnvollen Bereich einschr√§nken
  if (m<1 || m>12) return(0);                                   // Monat auf G√ºltigkeit pr√ºfen
  if (d<1 || d>mtag[m]+s*(m==2)) return(0);                     // Tag auf G√ºltigkeit pr√ºfen
  if (m < 3) { m += 13; y--; } else  m++;                       // Jahresanfang auf M√§rz verschieben
  s = d + 26*m/10 + y + y/4 - y/100 + y/400 + 6;                // eigentliche Berechnung
  return(s % 7 + 1);                                            // Ergebnis anpassen
}

///////////////////////////////////////////////////////

#endif












