void closeShtrB(){
  analogWrite(motorB_pwm, 255);
  digitalWrite(motorB_dir, LOW);
  delay(50);
  analogWrite(motorB_pwm, 0);
  shtrB = 0;
}

void openShtrB(){
  analogWrite(motorB_pwm,255);
  digitalWrite(motorB_dir, HIGH);
  delay(50);
  analogWrite(motorB_pwm, V_hold);
  delay(50);
  analogWrite(motorB_pwm, 0);
  shtrB = 1;
}

void flipShtrB(){
  analogWrite(motorB_pwm,255);
  PORTB ^= bit(motorB_dir_bit);
  delay(50);
  if (digitalRead(motorB_dir) == HIGH) {
    analogWrite(motorB_pwm, V_hold);
    delay(500);
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
