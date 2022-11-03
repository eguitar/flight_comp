#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include <SimpleKalmanFilter.h>

const int CounterMax = 90;
// 2s
const int DelayTime = 1000;
// passender 
const int CounterAdd = DelayTime / 1000;

SimpleKalmanFilter PressureKalmanFilter(1, 1, 0.01);
SimpleKalmanFilter TempKalmanFilter(1, 1, 0.01);
//SimpleKalmanFilter PressureKalmanFilter(0.5, 0.5, 0.01);
//SimpleKalmanFilter TempKalmanFilter(0.5, 0.5, 0.01);

#define SEALEVELPRESSURE_HPA (1013.25)
#define BMP_CS 36

Adafruit_BMP3XX bmp;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Adafruit BMP390");

//  if (!bmp.begin_I2C()) {   // hardware I2C mode, can pass in address & alt Wire
  if (! bmp.begin_SPI(BMP_CS)) {  // hardware SPI mode  
//  if (! bmp.begin_SPI(BMP_CS, BMP_SCK, BMP_MISO, BMP_MOSI)) {  // software SPI mode
    Serial.println("Could not find a valid BMP3xx sensor, check wiring!");
    while (1);
  }

  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_2X);
  // BMP3_OVERSAMPLING_2X
  // BMP3_No_OVERSAMPLING
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_32X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  //DISABLE);
  //COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_25_HZ);

  Serial.println("Sekunden,Temp,TempK,Press,PressK,Alt");
}

int Counter = 0;
 
void loop() {
  float Temp, TempK, Press, PressK, Alt;
  
  if (! bmp.performReading()) {
    Serial.println("Failed to perform reading :(");
    while (1);
    return;
  }

  Serial.print(Counter); Serial.print(",");
  Temp = bmp.temperature;
  //Serial.print("Temperature = ");
  Serial.print(Temp); Serial.print(",");
  TempK = TempKalmanFilter.updateEstimate(Temp);
  Serial.print(TempK); Serial.print(",");
  //Serial.println(" *C");

  Press = bmp.pressure / 100.0;
  //Serial.print("Pressure = ");
  Serial.print(Press); Serial.print(",");
  PressK = PressureKalmanFilter.updateEstimate(Press);
  Serial.print(PressK); Serial.print(",");
  //Serial.println(" hPa");

  Alt = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  //Serial.print("Approx. Altitude = ");
  Serial.println(Alt);
  //Serial.print(",");
  //Serial.println(" m");
  //Serial.println();
 
  switch (Counter) {
    case CounterMax:
      Serial.println("");
      break;
    default:
      // do not do anything
      break;
  }
  
  Counter = Counter + CounterAdd;
  
  delay(DelayTime);
}
