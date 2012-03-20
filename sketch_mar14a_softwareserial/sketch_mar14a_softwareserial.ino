/*TinyTracker 0.1
High-Altitude Balloon Payload Code
Last updated 15/03/2012
Released under the GNU/GPL License
by Alex Hutton*/
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "TinyGPS.h"
//http://ukhas.org.uk/guides:falcom_fsa03 modified tinygps?
#include "RTTY.h"
//Let's set up the objects
//Mark_Pin, Space_Pin, ASCII (7 or 8), Num Stop Bits (1 or 2), Baud (50 or 300)
RTTY rtty(4,5,7,1,50);
// rx, tx pins
SoftwareSerial gpsSerial(0, 1);
TinyGPS gps;
//name progress/new skills
// our sprintf buffer 
char superbuffer[100]=""; 
char callsign[]="HUTTON";
//we need some vars to hold GPS data in
unsigned long fix_age, time, date, speed, course, altitude;
unsigned long chars;
unsigned short sentences, failed_checksum;


//required arduino "setup" routine. this is run on poweron immediately.
void setup() {
gpsSerial.begin(4800);
}

//required arduino "loop" routine. this is run after the setup is complete. it loops.
void loop() {
  while (gpsSerial.available())
  {
    int c = gpsSerial.read();
    if (gps.encode(c))
    {
//process data
      long lat, lon;
      gps.get_position(&lat, &lon, &fix_age);
      gps.get_datetime(&date, &time, &fix_age);
      speed = gps.speed();
      altitude = gps.altitude(); //fix this
      course = gps.course();
//printf works like printf ("%d, %d, %d, %d, %d", 1Value, 2Value, , ,);
//$$<CALL SIGN>,<COUNTER D>,<TIME HH:MM:SS>,<LATITUDE DD.DDDDDD>,<LONGITUDE DD.DDDDDD>,<ALTITUDE METRES MMMMM>*<CHECKSUM><NEWLINE>
sprintf(superbuffer,"$$%d, %d, %d, %d, %d*%d",callsign,sentences,time,lat,lon,altitude);
    }
  }



}
