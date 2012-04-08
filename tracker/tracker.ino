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
//CONFIG
//RTTY CONFIG
const int PIN_RTTY_SPACE  = 2;
const int PIN_RTTY_MARK   = 3;
const int PIN_RTTY_ENABLE = 4; 	// Active High
//DEBUG CONFIG
const int debug           = 1; //debugging to software serial config (0=off, 1=only print rtty sentences, 2=echo GPS data as well)
const int debugRX         = 8; //DEBUG software serial RX pin
const int debugTX         = 9; //DEBUG software serial TX pin
const int dbgBaud         = 9600;//DEBUG software serial baud rate
//MISC CONFIG
const int checksum        = 0; //produce checksum for RTTY sentence (0=off, 1=on) NOT YET IMPLEMENTED
const int gpsBaud         = 9600; //baudrate of hardware serial port for GPS
const int bitRate         = 50;
const int PIN_PWR_LED     = 13;
//END CONFIG

//Setup objects
TinyGPS gps;
SoftwareSerial debugSerial(debugRX, debugTX);

//GPS variables (thanks Project Horus)
struct position_record // Total 25 bytes
{
  unsigned int            sequence_no;            // 2 bytes
  uint8_t                 hour;                           // 1 byte      
  uint8_t                 minute;                         // 1 byte
  uint8_t                 second;                         // 1 byte
  float                   latitude;                       // 4 bytes
  float                   longitude;                      // 4 bytes
  unsigned int            altitude;                       // 2 bytes
  unsigned int            velocity;                       // 2 bytes
  uint8_t                 sats;                           // 1 byte
  int8_t                  temp1; // Internal  // 1 byte
  int8_t                  temp2; // External  // 1 byte
  int8_t                 custom[5];                      // 5 bytes
};
position_record record1 = {0,0,0,0, 0.0,0.0, 0,0,0,0,0, {0,0,0,0,0}};//{0,1,2,3, -33.1234, 138.2345, 40575,200,10,4,-5, {0,0,0,0,0}};

// Output transmit Buffer.
char txBuffer[128];
char debugBuffer[128];
char latString[12];
char longString[12];

//setup
void setup() {
  //pin modes
  pinMode(PIN_RTTY_ENABLE, OUTPUT);
  pinMode(PIN_RTTY_SPACE, OUTPUT);
  pinMode(PIN_RTTY_MARK, OUTPUT); 
  pinMode(PIN_PWR_LED, OUTPUT);
  digitalWrite(PIN_RTTY_ENABLE, HIGH);
  digitalWrite(PIN_RTTY_MARK, HIGH);
  digitalWrite(PIN_RTTY_SPACE, LOW);
  digitalWrite(PIN_PWR_LED, LOW);
  
  
  if(debug > 0) debugSerial.begin(debugBaud); //if debug is enabled, set up the software serial port
  
  debugPrint("BOOTING. ENABLING GPS SERIAL");
  Serial.begin(gpsBaud); //set up the GPS serial port
  debugPrint("SERIAL ENABLED");
  
  debugPrint("CONFIG NMEA");
  configNMEA(); //disable all the NMEA crap we don't need (because we're using polling here)
  debugPrint("NMEA CONFIGURED");
}


void loop() {
  getGPS(); //polls the GPS and parses the data into the GPS data variables
  genSentence(); //takes the data and generates a sentence for transmission (eg; $$HUTTON,1,03:47:22,35.40024,-40.12334,6,10,0,0,0)
  debugPrint(txBuffer); //if debugging is enabled, sends the resulting sentence to the debug serial port
  txString(txBuffer); //finally, call the RTTY routine to checksum and then transmit the sentence.
}

void genSentence() {
  if(record1.sats != 0){
    dtostrf(record1.latitude, 11, 5, latString);
    dtostrf(record1.longitude, 11, 5, longString);
  }
  
  sprintf(txBuffer, "$$HUTTON,%u,%02u:%02u:%02d,%s,%s,%u,%u,%u,%d,%d", record1.sequence_no, record1.hour, record1.minute, record1.second, trim(latString), trim(longString), record1.altitude, record1.velocity, record1.sats, record1.temp1, record1.temp2);
  record1.sequence_no++;
}

void debugPrint(String msg) {
  if(debug != 0) debugSerial.println(msg);
}

void getGPS() {
  // get the GPS data
  feedGPS(); // up to 3 second delay here
  gps.f_get_position(&record1.latitude, &record1.longitude);
  record1.sats = (uint8_t)gps.sats();
  record1.altitude = (unsigned int)gps.f_altitude();
  record1.velocity = (unsigned int)gps.f_speed_kmph();
  gps.crack_datetime(0, 0, 0, &record1.hour, &record1.minute, &record1.second);
}
