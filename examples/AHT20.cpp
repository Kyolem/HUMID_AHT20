#include <Arduino.h>
#include <Ticker.h>
#include <Wire.h>
#include <AHT20.h>

#define AHT20_Addr 0x38

AHT20 HUMID;

const uint32_t PERIOD = 1000;

void action(void);

Ticker myTick(action, PERIOD, 0, MILLIS);

void action()
{
  if (HUMID.getValues())
  {
    Serial.print("Temperature : ");
    Serial.print(HUMID.theTemperature);
    Serial.println(" °C");
    Serial.print(" Humidity : ");
    Serial.print(HUMID.theHumidity);
    Serial.println(" %");
  }
  else
  {
    Serial.println("Erreur");
  }
}
void setup()
{

  Serial.begin(115200);
  Wire.begin();
  myTick.start();
  HUMID.begin(AHT20_Addr);
}

void loop()
{
  myTick.update();
}
