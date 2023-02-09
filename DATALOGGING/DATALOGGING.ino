 #include <SD.h>
#include <SPI.h>

const int delay_time = 500;

void setup()
{
  Serial.begin(9600);
  Serial.print("Initializing SD card...");

  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("card failed, or not present.\n");
    exit(0);
  }
  else {
    Serial.println("card initialized.\n");
  }
}

void loop()
{  
  String dataString = "data_string";

  File dataFile = SD.open("data_log.csv", FILE_WRITE);

  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    Serial.println(dataString);
  }
  else {
    // if the file isn't open, pop up an error:
    Serial.println("error opening datalogging file");
  }
  
  delay(delay_time); // run speed
}
