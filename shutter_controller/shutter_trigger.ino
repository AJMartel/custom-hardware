void trigSeq(){
  /* Trigger Mode */
  Serial.println("Trigger Mode Enabled");
  closeShtr();
  
  /* Get exposure parameters */
  int exp_time = Serial.parseInt();      // exposure time in microseconds
  float t_range = 256 * (exp_time + 9.2);  // total exposure time in microseconds
  Serial.print(exp_time);
  Serial.print("us, ");
  Serial.print(t_range/1000);
  Serial.println("ms");
  
  /* Trigger Loop */
  while (true) {
    
    /* exit from armed trigger mode */
    if (stringComplete) {
      if (inputString == "exit") {
        Serial.println("Exit Requested");
        break;
      }
      inputString = "";
      stringComplete = false;
    }
    
    /* wait for trigger */
    if (bitRead(PIND, extTrig_bit)){
      /* open shutter for exposure */
      analogWrite(motorPWM, 255);        // open shutter first to minimise opening delay
      bitSet(PORTB, motorDirection_bit);
      unsigned long t, t0 = millis();    // begin timing exposure once is shutter opening
      delay(100);
      analogWrite(motorPWM, V_hold);
      delay(50);
      analogWrite(motorPWM, 0);
      /* leave the shutter open for the full exposure */
      //delay(t_range/1000);
      while (t-t0 <= t_range/1000) {
        t = millis();
        if (t-t0 > 2000) break;
      }
      /* close shutter after exposure is complete */
      analogWrite(motorPWM, 255);
      bitClear(PORTB, motorDirection_bit);
      delay(50);
      analogWrite(motorPWM, 0);
      /* finish sequence */
      Serial.println("Triggered");
      break;
    }
    //else if (bitRead(PIND, endTrig_bit)) break;
    else if (!bitRead(PIND, shtrCtrl_bit)) break;
  }
  closeShtr();
  Serial.println("Sequence Complete");
}
