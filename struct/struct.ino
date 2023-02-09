// UCI RP - SOLIDS
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"

#define SEALEVELPRESSURE_HPA (1013.25)
#define BMP_CS 36

Adafruit_BMP3XX bmp;

const int delay_time = 500;

float alt, temp, press


void setup() {

  Serial.begin(9600);

  Serial.print("Initializing SD card...");

  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("card failed, or not present.\n");
    Serial.println("### -- Flight Computer Crashed. -- ###\n");
    exit(0);
  }

  Serial.println("card initialized.\n");

  Serial.println("Connecting to BMP390...");

  if (!bmp.begin_SPI(BMP_CS)) {
    Serial.println("sensor not found, check wiring!\n");
    Serial.println("### -- Flight Computer Crashed. -- ###\n");
    exit(0);
  }

  Serial.println("sensor found.\n");

  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_2X);
  // BMP3_OVERSAMPLING_2X
  // BMP3_No_OVERSAMPLING
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_32X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  //DISABLE);
  //COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_25_HZ);

  Serial.println("Altitude, Temperature, Pressure");

}


void loop() {

  if (! bmp.performReading()) {
    Serial.println("Sensor failed to perform reading.\n");
    Serial.println("### -- Flight Computer Crashed. -- ###\n");
    exit(0);
  }

  alt = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  temp = bmp.temperature;
  press = bmp.pressure / 100.0;

  String dataString = alt + "," + temp + "," + press;

  File dataFile = SD.open("data_log.csv", FILE_WRITE);

  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
  }
  else {
    Serial.println("Error Opening Data File.\n");
  }

  delay(delay_time);
}
