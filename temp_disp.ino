#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#include "INA3221.h"
#include <BH1750.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#define SERIAL_SPEED      115200    
#define PRINT_DEC_POINTS  3         
#define DHTPIN 4 
#define DHTTYPE DHT11

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);
INA3221 INA(0x40);
BH1750 lightMeter(0x23);

const int chipSelect = 10;
File dataFile;

void setup() {  

  Wire.begin();
  dht.begin();
  lcd.backlight();
  lcd.init();

  
  if (!INA.begin() )
  {
    Serial.println("could not connect. Fix and Reboot");
  }
  else
  {
    Serial.print("Found: \t");
    Serial.println(INA.getAddress());
  }

  INA.setShuntR(0, 0.100);
  lightMeter.begin();

  Serial.begin(9600);
  if (!SD.begin(chipSelect)) {
    Serial.println("Card initialization failed!");
    return;
  }
  Serial.println("Card initialized.");

  dataFile = SD.open("data.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println("Timestamp, SensorValue1, SensorValue2");
    dataFile.close();
  }
}


void loop() {
  
  //temperatura i wilgtonosc
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Wilgotnosc=");
   lcd.print((float)dht.readHumidity());
   lcd.print("%");
   lcd.setCursor(0,1);
   lcd.print("Temp=");
   lcd.print((float)dht.readTemperature());
   lcd.print("C");
   delay(2000);

   //prad i napiecie
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Prad=");
   lcd.print(INA.getCurrent_mA(1),3);
   lcd.print("mA");
   lcd.setCursor(0, 1);
   lcd.print("Napiecie=");
   lcd.print(INA.getBusVoltage(1),3);
   lcd.print("mV");
   delay(2000);

   //moc
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Moc=");
   lcd.print(INA.getPower_mW(1),3);
   lcd.print("mW");
   delay(2000);

   //nateznie swiatla
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("nat swiatla");
   lcd.setCursor(0, 1);
   lcd.print(lightMeter.readLightLevel());
   lcd.print("LUX");
   delay(2000);
   lcd.clear();

   String data = "Prad="+String(INA.getCurrent(1),3);
   writeDataToCSV(data);
   delay(1000);
}

void writeDataToCSV(String data) {
  dataFile = SD.open("data.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println(data);
    dataFile.close();
  }
}