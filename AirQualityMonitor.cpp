#include "Adafruit_PM25AQI.h"
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define WIRE Wire

SoftwareSerial pmSerial(2, 4);
Adafruit_PM25AQI aqi = Adafruit_PM25AQI();
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &WIRE);

int LED1 = 3;
int LED2 = 6;
int LED3 = 9;
int num_readings = 0;
int readings_total = 0;

void setup() {  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  Serial.begin(115200);
  while (!Serial) delay(10);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(1000);

  pmSerial.begin(9600);

  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);

  if (! aqi.begin_UART(&pmSerial)) {
    Serial.println("Could not find PM 2.5 sensor!");
    while (1) delay(10);
  }

  Serial.println("PM25 found!");
}

void loop() {
  PM25_AQI_Data data;

  if (! aqi.read(&data)) {
  Serial.println("Could not read from AQI");
  delay(500);
  return;
  }
  Serial.println("AQI reading success");

  display.clearDisplay();
  display.setCursor(0,0);

  num_readings++;
  readings_total += data.pm25_standard;

  display.print("Air Quality Monitor\n\n");
  display.print("PM 2.5: "); display.print(data.pm25_standard); display.print(" ug/m3\n");
  display.print("Avg PM 2.5: "); display.print(readings_total / num_readings); display.print(" ug/m3\n");

  display.display();

  Serial.println();
  Serial.println(F("---------------------------------------"));
  Serial.println(F("Concentration Units (standard)"));
  Serial.println(F("---------------------------------------"));
  Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_standard);
  Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_standard);
  Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_standard);
  Serial.println(F("Concentration Units (environmental)"));
  Serial.println(F("---------------------------------------"));
  Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_env);
  Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_env);
  Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_env);
  Serial.println(F("---------------------------------------"));
  Serial.print(F("Particles > 0.3um / 0.1L air:")); Serial.println(data.particles_03um);
  Serial.print(F("Particles > 0.5um / 0.1L air:")); Serial.println(data.particles_05um);
  Serial.print(F("Particles > 1.0um / 0.1L air:")); Serial.println(data.particles_10um);
  Serial.print(F("Particles > 2.5um / 0.1L air:")); Serial.println(data.particles_25um);
  Serial.print(F("Particles > 5.0um / 0.1L air:")); Serial.println(data.particles_50um);
  Serial.print(F("Particles > 10 um / 0.1L air:")); Serial.println(data.particles_100um);
  Serial.println(F("---------------------------------------"));
  

  if(data.pm25_standard <= 12){
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED1, HIGH);
  }
  else if(data.pm25_standard > 12 && data.pm25_standard < 35){
    digitalWrite(LED1, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED2, HIGH);
  }
  else{
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
  }
  

  delay(2000);
}
