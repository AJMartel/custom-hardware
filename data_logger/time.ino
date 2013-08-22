/* time declarations */
#include <Time.h>
#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message

boolean sync_status, prev_sync_status;

time_t requestSync() {
  Serial.write(TIME_REQUEST);  
  return 0;
  // the time will be sent later in response to serial mesg
}

void processSyncMessage() {
  unsigned long sync_time;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  // sync Arduino clock to the time received on the serial port
  if(Serial.find(TIME_HEADER)) {
     sync_time = Serial.parseInt();
     if (sync_time >= DEFAULT_TIME) setTime(sync_time);
  }
}

void checkSyncStatus() {
  /* check sync status */
  if  (timeStatus() != timeSet) {  // timeNotSet or timeNeedsSync
    prev_sync_status = sync_status;
    sync_status = false;
    
    /* moved these commands from setup to here */
    setSyncProvider(requestSync);  //set function to call when sync required
    setSyncInterval(60*60);
    
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
}
