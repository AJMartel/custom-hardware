void power_up(){
  // set all inputs high
  PORTD |= B11111100;
  PORTB |= B00000011;
  digitalWrite(power_clk, LOW);      // toggle power_clk low
  delay(10);                         // for 10 ms and then
  digitalWrite(power_clk, HIGH);     // revert to high
}

void power_down(){
  // set all inputs high
  PORTD |= B11111100;
  PORTB |= B00000011;
  digitalWrite(power, LOW);          // pull power low
  delay(10);                         // wait 10 ms
  digitalWrite(power_clk, LOW);      // toggle power_clk low
  delay(10);                         // for 10 ms and then
  digitalWrite(power_clk, HIGH);     // revert to high
}
