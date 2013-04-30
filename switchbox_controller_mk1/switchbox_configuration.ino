void config_status(){
  Serial.print("AC in = ");
  Serial.print(digitalRead(ACin));
  Serial.print(", AC out = ");
  Serial.println(digitalRead(ACout));
  Serial.print("DC in = ");
  Serial.print(digitalRead(DCin));
  Serial.print(", DC out = ");
  Serial.print(digitalRead(DCout));
  Serial.print(", DC amp = ");
  Serial.println(digitalRead(DCamp));
}

void reset(){
  PORTD &= B00000011; // disable AC mode (2 & 4)
  PORTB &= B00000000; // disable DC mode (8)
}
