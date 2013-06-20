#include <Sensirion.h>

const uint8_t dataPin  =  2;
const uint8_t clockPin =  3;

float temperature;
float humidity;
float dewpoint;

String inputString = "";
boolean stringComplete = false;

Sensirion tempSensor = Sensirion(dataPin, clockPin);

void setup()
{
  Serial.begin(9600);
  inputString.reserve(200);
}

void loop()
{
  /* Serial Commands and Communications */
  if (stringComplete) {
    tempSensor.measure(&temperature, &humidity, &dewpoint);
    if (inputString == "temp?") Serial.println(temperature);
    else if (inputString == "hum?") Serial.println(humidity);
    else if (inputString == "dew?") Serial.println(dewpoint);
    else if (inputString == "data?") {
      Serial.print(temperature);
      Serial.print(",");
      Serial.print(humidity);
      Serial.print(",");
      Serial.println(dewpoint);
    }
    inputString = "";
    stringComplete = false;
    delay(5000);
  }  
}
