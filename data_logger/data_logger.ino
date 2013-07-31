/* declarations */
#include <Time.h>
#define ACK 6    //ascii acknowledge
#define BEGIN_STREAM 1    // ASCII start of heading
#define END_STREAM 4      // ASCII end of transmission

/* serial declarations */
String inputString = "";
boolean stringComplete = false;

unsigned long log_time = 0, meas_time = 0, delay_time = 5000UL;
boolean logging = false;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(100);
  inputString.reserve(200);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  establishContact(); // requires computer to respond before proceeding
}

void loop() {
  if (logging) {
    /* moved these commands from setup to here */
    setSyncProvider(requestSync);  //set function to call when sync required
    setSyncInterval(60*60);
    /* check sync status */
    checkSyncStatus();
    /* check for logging serial commands */
    if (Serial.find("delay")) {
      delay_time = 1000UL * Serial.parseInt();
      Serial.write(ACK);}
    else if (stringComplete) {
      if (inputString == "stop") logging = false;
      inputString = "";
      stringComplete = false;}
    /* begin data logging */
    if (millis() - log_time >= delay_time) {
      log_time = millis();
      //Serial.write(BEGIN_STREAM);
      measure_temp();}
  }
  else {
    /* check for serial commands */
    if (stringComplete) {
      if (inputString == "start") logging = true;
      else if (inputString == "measure") {
        log_time = millis();
        measure_temp();}
    inputString = "";
    stringComplete = false;}
  }
}
