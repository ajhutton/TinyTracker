/*
  TinyTracker
  High-Altitude Balloon Payload Code
  Copyright 2012 Alex Hutton

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "TinyGPS.h"
#include "RTTY.h"
//CONFIG
//RTTY CONFIG
int    MARK     = 4; //RTTY MARK pin
int    SPACE    = 5; //RTTY SPACE pin
int    ASCII    = 7; //ASCII = 7 or 8
int    STOPBITS = 1; //STOPBITS = 1 or 2
int    BAUD     = 50;//BAUD = 50 or 300
//DEBUG CONFIG
int    debug    = 1; //debugging to software serial config (0=off, 1=only print rtty sentences, 2=echo GPS data as well)
int    debugRX  = 8; //DEBUG software serial RX pin
int    debugTX  = 9; //DEBUG software serial TX pin
int    dbgBaud  = 9600;//DEBUG software serial baud rate
//MISC CONFIG
String callsign = "HUTTON"; //balloon callsign for rtty transmission
int    checksum = 0; //produce checksum for RTTY sentence (0=off, 1=on) NOT YET IMPLEMENTED
int    gpsBaud  = 9600; //baudrate of hardware serial port for GPS
//END CONFIG

//GPS Data Variables
char superbuffer[100]=""; 
unsigned long fix_age, time, date, speed, course, altitude;
unsigned long chars;
unsigned short sentences, failed_checksum;
long lat, lon;

//Setup objects
RTTY rtty(MARK,SPACE,ASCII,STOPBITS,BAUD);
TinyGPS gps;
SoftwareSerial debugSerial(debugRX, debugTX);

void setup() {
Serial.begin(gpsBaud);
if(debug > 0) {
  debugSerial.begin(9600);
  debugSerial.print("Level ");
  debugSerial.print(debug);
  debugSerial.println(" debugging enabled. Logging to serial");
  }
}

void loop() {
  while (Serial.available())
  {
    int c = Serial.read();
    if (debug >= 2) debugSerial.write(c);
    if (gps.encode(c))
    {
      if(debug > 0) debugSerial.println("Got fix! Parsing...");
      //FILL VARS WITH GPS DATA
      gps.get_position(&lat, &lon, &fix_age);
      gps.get_datetime(&date, &time, &fix_age);
      speed = gps.speed();
      altitude = gps.altitude();
      course = gps.course();
      
      //GENERATE SENTENCE TO TRANSMIT
      char sent = sprintf(superbuffer,"$$%d,%d,%d,%d,%d,%d\n",callsign[1],sentences,time,lat,lon,altitude);
      if (debug >= 1) debugSerial.println(); debugSerial.println(superbuffer);
    }
  }



}
