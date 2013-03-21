void chopShtr(float f){
  float t = 1/f;
  analogWrite(motorPWM, 255);
  int i = 4;
  while(i--){
    bitSet(PORTB, motorDirection_bit);
    delay(t/2);
    bitClear(PORTB, motorDirection_bit);
    delay(t/2);
    Serial.println(i);
  }
  analogWrite(motorPWM, 0);
}

void closeShtr(){
  analogWrite(motorPWM, 255);
  digitalWrite(motorDirection, LOW);
  delay(50);
  analogWrite(motorPWM, 0);
  shtr = 0;
}

void openShtr(){
  analogWrite(motorPWM,255);
  digitalWrite(motorDirection, HIGH);
  delay(50);
  analogWrite(motorPWM, V_hold);
  delay(50);
  analogWrite(motorPWM, 0);
  shtr = 1;
}

void flipShtr(){
  analogWrite(motorPWM,255);
  PORTB ^= bit(motorDirection_bit);
  delay(50);
  if (digitalRead(motorDirection) == HIGH) {
    analogWrite(motorPWM, V_hold);
    delay(500);
    analogWrite(motorPWM, 0);
  }
  else analogWrite(motorPWM, 0);
  shtr ^= 1;
  Serial.print("Shutter ");
  if (shtr == 0) Serial.println("Closed");
  else Serial.println("Open");
}
