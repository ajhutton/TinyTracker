/*TinyTracker 0.1
High-Altitude Balloon Payload Code
Released under the GNU/GPL License
by Alex Hutton*/
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
int    checksum = 0; //produce checksum for RTTY sentence (0=off, 1=on)
int    gpsBaud  = 9600; //baudrate of hardware serial port for GPS
//END CONFIG

//GPS Data Variables
char superbuffer[100]=""; 
unsigned long fix_age, time, date, speed, course, altitude;
unsigned long chars;
unsigned short sentences, failed_checksum;

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

//required arduino "loop" routine. this is run after the setup is complete. it loops.
void loop() {
  while (Serial.available())
  {
    int c = Serial.read();
    if (debug >= 2) debugSerial.write(c);
    if (gps.encode(c))
    {
      if(debug > 0) debugSerial.println("Got fix! Parsing...");
      long lat, lon;
      gps.get_position(&lat, &lon, &fix_age);
      gps.get_datetime(&date, &time, &fix_age);
      speed = gps.speed();
      altitude = gps.altitude(); //fix this
      course = gps.course();
      char sent = sprintf(superbuffer,"$$%d,%d,%d,%d,%d,%d*%d\n",callsign[1],sentences,time,lat,lon,altitude,checksum);
      if (debug >= 1) debugSerial.println(); debugSerial.println(superbuffer);
    }
  }



}
