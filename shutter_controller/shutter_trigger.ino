void trigSeq(){
  /* Trigger Mode */
  Serial.println("Trigger Mode Enabled");
  closeShtr();
  
  /* Get exposure parameters */
  int exp_time = Serial.parseInt();
  int t_range = 256 * (exp_time + 9.2);
  Serial.print(exp_time);
  Serial.print(", ");
  Serial.println(t_range/1000);
  
  /* Trigger Loop */
  int trig = 0;
  while (!trig) {
    
    if (Serial.available() > 0) {
      Serial.println("Exit Requested");
      int inByte = Serial.read();
      switch (inByte) {
      case 'e':
        trig = 1;
      }
    }
    
    if (bitRead(PIND, extTrig_bit)){
      analogWrite(motorPWM,255);
      bitSet(PORTB, motorDirection_bit);
      //delayMicroseconds(t_range);
      delay(t_range/1000);
      bitClear(PORTB, motorDirection_bit);
      //delay(50);
      //analogWrite(motorPWM, V_hold);
      delay(50);
      analogWrite(motorPWM, 0);
      Serial.println("Triggered");
      break;
    }
    //else if (bitRead(PIND, endTrig_bit)) break;
    else if (!bitRead(PIND, shtrCtrl_bit)) break;
  }
  closeShtr();
  Serial.println("Sequence Complete");
}
