#include <SD.h>
#include <SPI.h>

const int chipSelect = BUILTIN_SDCARD;
int count = 0;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
//  while (!Serial) {
//    ; // wait for serial port to connect.
//  }

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while (1) {
      // No SD card, so don't do anything more - stay stuck here
    }
  }
  Serial.println("Card Initialized.");
}

void loop()
{
  if (count == 20) {
    exit(0);
  }
  else {
    count++;
  }
  
  // make a string for assembling the data to log:
  String data_out = "";

  // read three sensors and append to the string:
  for (int analogPin = 0; analogPin < 3; analogPin++) {
//    int sensor = analogRead(analogPin);
    data_out += String(analogPin);
    if (analogPin < 2) {
      data_out += ",";
    }
  }

  // open the file.
  File dataFile = SD.open("data_file.csv", FILE_WRITE);

  if (dataFile) {
    dataFile.println(data_out);
    dataFile.close();
    Serial.println(data_out);
  }
  else {
    Serial.println("Error opening datalog.txt");
  }
  delay(100); // run at a reasonable not-too-fast speed
}
