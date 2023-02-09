#include <Arduino.h>
#include "TeensyThreads.h"
//----------------------------------
const int LED = 13;
volatile int blinkcode = 0;
//----------------------------------
#include <SD.h>
#include <SPI.h>
//----------------------------------
const int chipSelect = BUILTIN_SDCARD;




//----------------------------------
void blinkthread() {
  while(1) {
    // BLINK ---------------------------------------------
    if (blinkcode) {
      for (int i=0; i<blinkcode; i++) {
        digitalWrite(LED, HIGH);
        threads.delay(350);
        digitalWrite(LED, LOW);
        threads.delay(1000);
      }
      blinkcode = 0;
    }
    threads.yield();
  }
}
//----------------------------------




//----------------------------------
void setup() {
  // BLINK -------------------------
  delay(1000);
  pinMode(LED, OUTPUT);
  threads.addThread(blinkthread);

  // DATALOG -----------------------
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect.
  }
  
  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while (1) {
      exit(0); // No SD card, so don't do anything more - stay stuck here
    }
  }
  Serial.println("card initialized.");
}
//----------------------------------


int count = 0;



//----------------------------------
void loop() {

  // DATALOG -------------------------------------------
  String dataString = "";

  // read three sensors and append to the string:
  for (int analogPin = 0; analogPin < 3; analogPin++) {
    dataString += String(analogPin);
    if (analogPin < 2) {
      dataString += ",";
    }
  }

  // open the file.
  File dataFile = SD.open("data_file.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  } else {
    // if the file isn't open, pop up an error:
    Serial.println("error opening datalog.txt");
  }
  
  if (count == 8) {
    exit(0);
  }
  count++;
  blinkcode = count;
  delay(1000);
  Serial.println(count);
}
