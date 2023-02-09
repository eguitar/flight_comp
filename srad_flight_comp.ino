#include <SST25VF.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"

#define DL 6
#define SEALEVELPRESSURE_HPA (1013.25)   //change this according to location and day
                                         //1 inHg = 33.863886666667 hPa

//defining pins for bmp388 barometer
#define BMP_SCK 13   //SCL 13
#define BMP_MISO 12  //SDO 12
#define BMP_MOSI 11  //SDA 11
#define BMP_CS 10    //CS 10

//defining pins for sst25vf080b serial flash
#define memCE SS          //(pin 1)chip enable
#define memSO MISO        //(pin 2)Serial Data Output
//#define memWP           //(pin 3)Write Protect           (leave as //, is to 3.3v)
//Vss                     //(pin 4)Ground                  (leave as //)
#define memSI MOSI        //(pin 5)Serial Data Input
#define memSCK SCK        //(pin 6)Serial Data Input
//#define mem_HOLD        //(pin 7)Hold                    (leave as //, is to 3.3v)
//Vdd                     //(pin 8)2.7-3.6V Power Supply   (leave as //, is to 3.3v)

// consult with Hala and Jennifer ////////////////////////////////////////////////////////////

int launch_flag = 0;
int drogue_flag = 0;
int decline_counter = 0;

float past_alt = 0;
float main_alt = 1500;
float alt = 0;
float acc = 0;

  
void setup() {

  // check with Hala and Jennifer ///////////////////////////////////////////////////////////
  // components not yet decided ^

}

void loop() {

  if ( launch_flag == 0 ) {
    if ( getAccelero() >= 3g ) {                 // condition for when rocket is launched
      launch_flag = 1;
    }
  }

  else {
    alt = getAltitude();                          // data logging
    acc = getAccelero();
    saveAltitude(alt);
    saveAccelero(acc);
    if ( !drogue_flag ) {                        // check for decrementing altitude
      if ( alt - past_alt < 0 ) {
        decline_counter++;
      }
      else {
        decline_counter = 0;
      }
    }
    if ( !drogue_flag && decline_counter > 2 ) { // condition checking for rocket falling
      setMOSFET(drogue)
      drogue_flag = 1;
    }
    if ( drogue_flag && alt <= main_alt ) {      // condition to launch main chute
      setMOSFET(main);
    }
    past_alt = alt;
  }
}


float getAltitude() {}

float getAccelero() {}

void saveAltitude(float alt) {}

void saveAccelero(float acc) {}

void setMOSFFET() {}
