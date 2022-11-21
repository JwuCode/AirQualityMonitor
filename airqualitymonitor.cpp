#include "Adafruit_PM25AQI.h"
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define WIRE Wire

SoftwareSerial pmSerial(2, 3);
Adafruit_PM25AQI aqi = Adafruit_PM25AQI();
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &WIRE);

int LED1 = 3;
int LED2 = 6;
int LED3 = 9;

void setup()
{
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  Serial.begin(115200);
  while (!Serial)
    delay(10);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(1000);

  pmSerial.begin(9600);

  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  if (!aqi.begin_UART(&pmSerial))
  {
    Serial.println("Could not find PM 2.5 sensor!");
    while (1)
      delay(10);
  }

  Serial.println("PM25 found!");
}

void loop()
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Air Quality Monitor\n\n");
  display.display();

  PM25_AQI_Data data;

  /*if (! aqi.read(&data)) {
    Serial.println("Could not read from AQI\n");
    delay(500);
    return;
  }
  Serial.println("AQI reading success");*/

  data.pm25_standard = 50;

  Serial.print(F("PM 2.5: "));
  Serial.print(data.pm25_standard);
  Serial.print(" ug/m3\n");
  display.print("PM 2.5: ");
  display.print(data.pm25_standard);
  display.print(" ug/m3\n");
  display.display();

  if (data.pm25_standard <= 12)
  {
    digitalWrite(LED1, HIGH);
    delay(1000);
    digitalWrite(LED1, LOW);
  }
  else if (data.pm25_standard > 12 && data.pm25_standard < 35)
  {
    digitalWrite(LED2, HIGH);
    delay(1000);
    digitalWrite(LED2, LOW);
  }
  else
  {
    digitalWrite(LED3, HIGH);
    delay(1000);
    digitalWrite(LED3, LOW);
  }

  delay(1000);
}