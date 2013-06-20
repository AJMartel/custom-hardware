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
     if( sync_time >= DEFAULT_TIME) setTime(sync_time);
  }
}
