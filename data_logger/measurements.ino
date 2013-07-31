/* temperature sensor declarations */
#include <Sensirion.h>
const int dataPin  =  2;
const int clockPin =  3;
float temperature, humidity, dewpoint;
Sensirion tempSensor = Sensirion(dataPin, clockPin);

void measure_temp() {
  tempSensor.measure(&temperature, &humidity, &dewpoint);
  Serial.print(now());
  Serial.print(",");
  Serial.print(temperature);
  Serial.print(",");
  Serial.println(humidity);
}

void check_delay() {
  int t = millis();
  delay(delay_time);
  Serial.print("Arduino: measurement period = ");
  Serial.print(millis() - t);
  Serial.println(" ms");
}
