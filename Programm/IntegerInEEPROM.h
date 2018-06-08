/*
 *	This is a library for saving and reading integer variables from the EEPROM.
 *
 *	The Code is written by Matthias Busse,
 *	and the library is put together by Ralf Rumbler, DO3KV on 30.5.2018 .
 */

#ifndef IntegerInEEPROM_h
#define IntegerInEEPROM_h

#include <Arduino.h>
#include <EEPROM.h>

int eepromReadInt(int adr) {
  // 2 Byte Integer Zahl aus dem EEPROM lesen an der Adresse
  // Eingabe:
  //   adr: Adresse +0 und +1 wird gelesen
  // Ausgabe: int Wert
  //
  // Matthias Busse 5.2014 V 1.0

  byte low, high;

  low=EEPROM.read(adr);
  high=EEPROM.read(adr+1);
  return low + ((high << 8)&0xFF00);
}

void eepromWriteInt(int adr, int wert) {
  // 2 Byte Integer Zahl im EEPROM ablegen an der Adresse
  // Eingabe:
  //   adr: Adresse +0 und +1 wird geschrieben
  //   wert: möglicher Wertebereich -32,768 bis 32,767
  // Ausgabe:
  //   -
  // 2 Byte Platz werden belegt.
  //
  // Matthias Busse 5.2014 V 1.0
  //
  // (Speicher von Arduino UNO (ATmega328P) = 1024 Bytes)

  byte low, high;

  low=wert&0xFF;
  high=(wert>>8)&0xFF;
  EEPROM.write(adr, low); // dauert 3,3ms
  EEPROM.write(adr+1, high);
  return;
}

#endif











