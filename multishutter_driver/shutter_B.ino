void closeShtrB(){
  digitalWrite(motorB_brake, LOW);
  analogWrite(motorB_pwm, 255);
  digitalWrite(motorB_dir, LOW);
  delay(50);
  digitalWrite(motorB_brake, HIGH);
  delay(100);
  analogWrite(motorB_pwm, 0);
  shtrB = 0;
}

void openShtrB(){
  digitalWrite(motorB_brake, LOW);
  analogWrite(motorB_pwm,255);
  digitalWrite(motorB_dir, HIGH);
  delay(50);
  digitalWrite(motorB_brake, HIGH);
  delay(100);
  analogWrite(motorB_pwm, 0);
  shtrB = 1;
}

void flipShtrB(){
  digitalWrite(motorB_brake, LOW);
  analogWrite(motorB_pwm,255);
  PORTB ^= bit(motorB_dir_bit);
  delay(50);
  digitalWrite(motorB_brake, HIGH);
  delay(100);
  if (digitalRead(motorB_dir) == HIGH) {
    //analogWrite(motorB_pwm, V_hold);
    //delay(50);
    analogWrite(motorB_pwm, 0);
  }
  
  else analogWrite(motorB_pwm, 0);
  shtrB ^= 1;
}

void statusShtrB(){
  Serial.print("Shutter B ");
  if (shtrB == 0) Serial.println("Closed");
  else Serial.println("Open");
}
