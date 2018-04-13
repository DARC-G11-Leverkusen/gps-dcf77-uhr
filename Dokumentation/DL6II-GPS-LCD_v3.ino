/*
   GPS Clock by IGEL e.V. Leverkusen
   Author: Vincent Stallbaum, DL6II

   This clock can be used with many GPS module.
   The code is optimised for a 16*2 standart LCD.

   Feel free to share and optmize the code
   Contact: dl6ii@vsas.de
 */

#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <math.h>

LiquidCrystal lcd(3,5,9,10,11,12);


SoftwareSerial mySerial(A3, A2); // TX, RX
Adafruit_GPS GPS(&mySerial);

#define GPSECHO  false
#define DSTPIN A1
#define ROUND_US(d) ( (int) ((d) + ((d) > 0 ? 0.5 : -0.5)) )

// this keeps track of whether we're using the interrupt
// off by default!

const int buttonPin=1; 

// Offset hourtemp from gps time (UTC)
const int offset = 0;   // Central European Time

boolean usingInterrupt = false;
void useInterrupt(boolean); // Func prototype keeps Arduino 0023 happy

int buttonState=0; 
int lastButtonState=0;
int menue=2;
int montharray[]={31,28,31,30,31,30,31,31,30,31,30,31};
int secondtemp;
int minutetemp;
int hourtemp;
int daytemp;
int monthtemp;
int yeartemp;
int speadkmh;
int altitudem;
int angled;
byte leap;
int lastsecondtemp;
long lat, lon;

// the following variables are unsigned long's because the time, measured in milisecondtemp,
// will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup()  
{
  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(115200);

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);

  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
   GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  // GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time

  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  // the nice thing about this code is you can have a timer0 interrupt go off
  // every 1 millisecond, and read data from the GPS for you. that makes the
  // loop code a heck of a lot easier!
  useInterrupt(true);

  // delay(1000);
  // Ask for firmware version
  // mySerial.println(PMTK_Q_RELEASE);

  lcd.begin(16,2);
  pinMode(buttonPin, INPUT); 
}
// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
#ifdef UDR0
  if (GPSECHO)
    if (c) UDR0 = c;  
  // writing direct to UDR0 is much much faster than Serial.print 
  // but only one character can be written at a time. 
#endif
}


void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } 
  else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}
uint32_t timer = millis();
uint32_t animationTimer = millis();


void loop() {                     // run over and over again
                                  // in case you are not using the interrupt above, you'll
                                  // need to 'hand query' the GPS, not suggested :(
  if (! usingInterrupt) {         // read data from the GPS in the 'main loop'
    char c = GPS.read();          // if you want to debug, this is a good time to do it!
    if (GPSECHO)
      if (c) Serial.print(c);
  }
  
  if (GPS.newNMEAreceived()) {        // if a sentence is received, we can check the checksum, parse it...
                                      // a tricky thing here is if we print the NMEA sentence, or data
                                      // we end up not listening and catching other sentences! 
                                      // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
                                      //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;                         // we can fail to parse a sentence in which case we should just wait for another
  }
  if (timer > millis())  timer = millis();      // if millis() or timer wraps around, we'll just reset it
  if (animationTimer > millis())  animationTimer = millis();

                                      // approximately every .1 secondtemp or so, print out the current stats
                                      // if (millis() - timer > 100) { 
                                      // timer = millis(); // reset the timer
                                      
  int reading = digitalRead(buttonPin); // read the state of the switch into a local variable:

                                        // check to see if you just pressed the button
                                        // (i.e. the input went from LOW to HIGH),  and you've waited
                                        // long enough since the last press to ignore any noise:
                                        // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {     // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {  // whatever the reading is at, it's been there for longer   
                                                        // than the debounce delay, so take it as the actual current state:
                                                        // if the button state has changed:   
    if (reading != buttonState) {
      buttonState = reading;                            // only toggle the menue if the new button state is HIGH      
      if (buttonState == HIGH) {
        //menueup = !menueup;
        menue=menue+1;
      }
    }
  }
  if (menue>2) {
    menue=0;
  }

  lastButtonState = reading;        // save the reading.  Next time through the loop,it'll be the lastButtonState:
  displayhamgps();
}


void displayhamgps(){  
  if (millis() - timer > 250) { 
    timer = millis(); // reset the timer
    if (GPS.fix) {

      //secondtemp = GPS.seconds;
      minutetemp = GPS.minute;
      hourtemp = GPS.hour;
      daytemp = GPS.day;
      monthtemp = GPS.month;
      yeartemp = GPS.year;      
      leapcheck();
      dst();
      timezone(0);    

      speadkmh = ROUND_US(GPS.speed * 1.852);
      altitudem = ROUND_US(GPS.altitude);
      angled = ROUND_US(GPS.angle);
      
      //Maidenhead Calculation                                                                  
      lat = GPS.latitude_fixed/100;
      lon = GPS.longitude_fixed/100;
      lon = lon + 18000000;                            // Step 1
      lat = lat +  9000000;                            // Adjust so Locn AA is at the pole
      char MH[6] = {'A', 'A', '0', '0', 'a', 'a'};     // Initialise our print string
        MH[0] +=  lon / 2000000;                       // Field
        MH[1] +=  lat / 1000000;
        MH[2] += (lon % 2000000) / 200000;             // Square
        MH[3] += (lat % 1000000) / 100000;
        MH[4] += (lon %  200000) /   8333;             // Subsquare .08333 is   5/60 
        MH[5] += (lat %  100000) /   4166;             //           .04166 is 2.5/60  
      String MH_txt = "";                              // Build up Maidenhead
      int i = 0;                                       // into a string that's easy to print
      while (i < 6){ 
        MH_txt += MH[i];
        i++; }
        
      // print time and date
      displayclock();  
          
      // print buttom line
      lcd.setCursor(0,1);
/*
      if (GPS.minute < 20) lcd.print('0');

      if (minutetemp < 20){ 
        menue=2;
        }
        else if (minutetemp < 40){
          menue=1;
        }
        else menue=0;
 */              





                                   
      switch (menue) {
        case 0:{
          lcd.print(MH_txt);
          lcd.print(" "); 
          if (altitudem < 1000) lcd.print(' ');
          if (altitudem < 100) lcd.print(' ');
          if (altitudem < 10) lcd.print(' ');   
          lcd.print(altitudem);
          lcd.print("m");  
          lcd.print("  ");
          if (GPS.satellites < 10) lcd.print('0'); 
          lcd.print(GPS.satellites);
        }
        break;
        case 1:{
          if ((GPS.latitude_fixed/10000000.0) < 10) lcd.print(' ');   
          lcd.print((GPS.latitude_fixed/10000000.0),4);
          lcd.print(GPS.lat);
          lcd.print("");
          if ((GPS.longitude_fixed/10000000.0) < 10) lcd.print(' ');   
          lcd.print((GPS.longitude_fixed/10000000.0),4);
          lcd.print(GPS.lon);
        }
        break;
        case 2:{
          if (angled < 100) lcd.print('0');
          if (angled < 10) lcd.print('0');
          lcd.print(GPS.angle);
          lcd.print("\337");  
          lcd.print("  ");
          if (speadkmh < 100) lcd.print(' ');
          if (speadkmh < 10) lcd.print(' ');             
          lcd.print(speadkmh);
          lcd.print("km/h"); 
        }
        break;       
        default: lcd.print("****************");
        break;
     }
    }
  }
  if (!GPS.fix){ // if no fix, display animation
    displayclock();
    lcd.setCursor(0,1);
    lcd.print("  LOCATING");
    lcd.setCursor(15,1);
    lcd.print("  ");
    lcd.setCursor(10,1);
    if (millis() - animationTimer > 1000) lcd.print(".");
    if (millis() - animationTimer > 2000) lcd.print(".");
    if (millis() - animationTimer > 3000) lcd.print(".");
    if (millis() - animationTimer > 4000) lcd.print(".  ");
    if (millis() - animationTimer > 5000){
      animationTimer = millis(); // reset the timer
      lcd.setCursor(10,1); 
      lcd.print("    "); // clear the .'s
      //      secondtemp = secondtemp+5;
     }
  }
}


void displayclock(){ // print time
      lcd.setCursor(0,0);
      if (hourtemp <10) lcd.print('0');
      lcd.print(hourtemp, DEC); 
      lcd.print(':');
      if (GPS.minute < 10) lcd.print('0');
      lcd.print(GPS.minute, DEC); 
      lcd.print(':');
      if (GPS.seconds < 10) lcd.print('0');
      lcd.print(GPS.seconds, DEC); 
      lcd.print("  "); 
      // print date
      if (daytemp < 10) lcd.print('0');
      lcd.print(daytemp, DEC); 
      lcd.print(".");
      if (monthtemp < 10) lcd.print('0');
      lcd.print(monthtemp, DEC); 
      lcd.print('.');
      //lcd.print(yeartemp, DEC);   
}


void leapcheck(){  // check for leap year
  yeartemp=2000+yeartemp;
  leap = yeartemp%4 == 0 && yeartemp%100 !=0 || yeartemp%400 == 0;
  if (leap > 0) montharray[1]=29;
  yeartemp=yeartemp-2000;
}


void timezone(int tzone){  // set time zone and fix date when wrong
  hourtemp = hourtemp + tzone;
  
  // for time zones UTC-
  if (hourtemp < 0){
    hourtemp= 24+hourtemp;
    if (daytemp==1){ //fix getting to the first day of the month too soon
      daytemp=(montharray[monthtemp-2]);  // montharray[]={31,28 (or29) ,31,30,31,30,31,31,30,31,30,31};
      if (monthtemp==1){ //Fix year if UTC is Jan 1st and we're not there yet
        daytemp=31; 
        monthtemp=12; 
        yeartemp=yeartemp-1;
      } 
      else monthtemp=monthtemp-1;
    } 
    else daytemp=daytemp-1;
  }

  // for time zones UTC+
  if (hourtemp > 24){
    hourtemp= hourtemp-24;
    if (daytemp==montharray[monthtemp-1]){ // fix not changing month soon enough
      daytemp==1;
      if (monthtemp==12){ // Fix year if UTC is Dec 31st and we're past that
        daytemp=1; 
        monthtemp=1; 
        yeartemp=yeartemp+1;
      } 
      else monthtemp=monthtemp+1;
    } 
    else daytemp=daytemp+1;
  }

}


void dst(){
  if (digitalRead(DSTPIN)==HIGH) hourtemp=hourtemp+1;
  if (hourtemp==24) hourtemp=0;
}


void twelvehour(){
  if (hourtemp>12) hourtemp=hourtemp-12;
  if (hourtemp==0) hourtemp=12;
}

