#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>

const int chipSelect = 10; 
static const int RXPin = 3, TXPin = 4;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);
  
    if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
    File dataFile = SD.open("rundata.csv", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    
    dataFile.println("Latitude, Longitude, Altitude, Velocidade\n"); // Change this text according
    // to what you are measuring; it works as a header for the saved data! 
    dataFile.close();
    }  
  // if the file isn't open, pop up an error:
  else {
    //blindversion Serial.println("error opening novdat.csv");
  }
  Serial.println("card initialized.");

  Serial.println(F("FullExample.ino"));
  Serial.println(F("An extensive example of many interesting TinyGPS++ features"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
  Serial.println(F("Latitude   Longitude  Date       Time"));
  Serial.println(F("(deg)      (deg)                     "));
  Serial.println(F("---------------------------------------------------------------------------------------------------------------------------------------"));
}

void loop()
{
  

  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
  printDateTime(gps.date, gps.time);
  printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
  printFloat(gps.speed.kmph(), gps.speed.isValid(), 6, 2);
  
   //---------------------------------------------------------------------------
   // The routine for writing data to SDcard: 
    File dataFile = SD.open("rundata.csv", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    
    dataFile.print(gps.location.lat(),2,6);
    dataFile.print(",");
    dataFile.print(gps.location.lng(),2,6);
    dataFile.print(",");
    //dataFile.print(gps.date);
    //dataFile.print(",");    
    //dataFile.print(gps.time);
    //dataFile.print(",");
    dataFile.print(gps.altitude.meters());
    dataFile.print(",");
    dataFile.print(gps.speed.kmph());
    dataFile.print("\n");
    
    dataFile.close();
    // print to the serial port too:
    //blindversion Serial.println(bmp.readTemperature());
    //blindversion Serial.println(GetTemp(),1);
  }  
  // if the file isn't open, pop up an error:
  else {
    //blindversion Serial.println("error opening novdat.csv");
  }
  //---------------------------------------------------------------------------
 
 Serial.println();
  
  smartDelay(2000);

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
}

// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
}

static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  smartDelay(0);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
  if (!d.isValid())
  {
    Serial.print(F("********** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
    Serial.print(sz);
  }
  
  if (!t.isValid())
  {
    Serial.print(F("******** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    Serial.print(sz);
  }

  printInt(d.age(), d.isValid(), 5);
  smartDelay(0);
}

/* static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartDelay(0);
}*/
