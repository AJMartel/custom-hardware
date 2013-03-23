void closeShtrA(){
  analogWrite(motorA_pwm, 255);
  digitalWrite(motorA_dir, LOW);
  delay(50);
  analogWrite(motorA_pwm, V_hold);
  delay(1000);
  analogWrite(motorA_pwm, 0);
  shtrA = 0;
}

void openShtrA(){
  analogWrite(motorA_pwm,255);
  digitalWrite(motorA_dir, HIGH);
  delay(50);
  analogWrite(motorA_pwm, V_hold);
  delay(50);
  analogWrite(motorA_pwm, 0);
  shtrA = 1;
}

void flipShtrA(){
  analogWrite(motorA_pwm,255);
  PORTB ^= bit(motorA_dir_bit);
  delay(50);
  if (digitalRead(motorA_dir) == HIGH) {
    analogWrite(motorA_pwm, V_hold);
    delay(500);
    analogWrite(motorA_pwm, 0);
  }
  else analogWrite(motorA_pwm, 0);
  shtrA ^= 1;
}

void statusShtrA(){
  Serial.print("Shutter A ");
  if (shtrA == 0) Serial.println("Closed");
  else Serial.println("Open");
}
