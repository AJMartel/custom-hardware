void closeShtrs(){
  analogWrite(motorA_pwm, 255);
  analogWrite(motorB_pwm, 255);
  digitalWrite(motorA_dir, LOW);
  digitalWrite(motorB_dir, LOW);
  delay(50);
  analogWrite(motorA_pwm, 0);
  analogWrite(motorB_pwm, 0);
  shtrA = 0; shtrB = 0;
}

void openShtrs(){
  analogWrite(motorA_pwm,255);
  analogWrite(motorB_pwm,255);
  digitalWrite(motorA_dir, HIGH);
  digitalWrite(motorB_dir, HIGH);
  delay(50);
  analogWrite(motorA_pwm, V_hold);
  analogWrite(motorB_pwm, V_hold);
  delay(50);
  analogWrite(motorA_pwm, 0);
  analogWrite(motorB_pwm, 0);
  shtrA = 1; shtrB = 1;
}

void flipShtrs(){
  analogWrite(motorA_pwm,255);
  analogWrite(motorB_pwm,255);
  PORTB ^= bit(motorA_dir_bit);
  PORTB ^= bit(motorB_dir_bit);
  delay(50);
  if (digitalRead(motorA_dir) == HIGH) {
    analogWrite(motorA_pwm, V_hold);
    delay(500);
    analogWrite(motorA_pwm, 0);
  }
  else analogWrite(motorA_pwm, 0);
  shtrA ^= 1;
  if (digitalRead(motorB_dir) == HIGH) {
    analogWrite(motorB_pwm, V_hold);
    delay(500);
    analogWrite(motorB_pwm, 0);
  }
  else analogWrite(motorB_pwm, 0);
  shtrB ^= 1;
}

void statusShtrs(){
  Serial.print("Shutter A ");
  if (shtrA == 0) Serial.print("Closed,");
  else Serial.print("Open, ");
  Serial.print("Shutter B ");
  if (shtrB == 0) Serial.println("Closed");
  else Serial.println("Open");
}
