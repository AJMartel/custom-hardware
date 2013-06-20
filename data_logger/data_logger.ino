/* time declarations */
#include <Time.h>  
#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message
#define ACK 6    //ascii acknowledge
#define BEGIN_STREAM 1    // ASCII start of heading
#define END_STREAM 4      // ASCII end of transmission

/* serial declarations */
String inputString = "";
boolean stringComplete = false;

unsigned long log_time = 0, meas_time = 0, delay_time = 5000UL;
boolean sync_status, prev_sync_status;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(100);
  inputString.reserve(200);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  // requires computer to respond before proceeding
  establishContact();
  setSyncProvider(requestSync);  //set function to call when sync required
  setSyncInterval(60*60);
}

void loop() {
  /* check sync status */
  if  (timeStatus() != timeSet) {  // timeNotSet or timeNeedsSync
    prev_sync_status = sync_status;
    sync_status = false;
    digitalWrite(13, LOW);
    if (timeStatus() == timeNotSet) {
      while (timeStatus() != timeSet) processSyncMessage();
      if (timeStatus() == timeSet) Serial.write(ACK);
    }
    else if (timeStatus() == timeNeedsSync) processSyncMessage();
  }
  else if (timeStatus() == timeSet) {
    prev_sync_status = sync_status;
    sync_status = true;
    if (prev_sync_status == false) digitalWrite(13, HIGH);
  }
  
  /* check for serial commands */
  if (Serial.find("delay")) {
    delay_time = 1000UL * Serial.parseInt();
    Serial.write(ACK);}
  else if (stringComplete) {
    if (inputString == "measure") {
      log_time = millis();
      measure();}
    inputString = "";
    stringComplete = false;
  }
  
  /* begin data logging */
  if (millis() - log_time >= delay_time) {
    log_time = millis();
    //Serial.write(BEGIN_STREAM);
    measure();
  }
}
