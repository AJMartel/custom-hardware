void clk_through(){
  digitalWrite(clk, LOW);      // toggle clk low
  delay(10);                         // for 10 ms and then
  digitalWrite(clk, HIGH);     // revert to high
}

void switch_AC(){
  power_up();     // power up
  // disable DC in, out and amp
  digitalWrite(DCin, LOW);
  digitalWrite(DCout, LOW);
  digitalWrite(DCamp, LOW);
  // enable AC in and out
  digitalWrite(ACin, HIGH);
  digitalWrite(ACout, HIGH);
  clk_through();  // change relay configuration
  power_down();   // power down
}

void switch_DC(){
  power_up();     // power up
  // disable AC in, out and DC amp
  digitalWrite(ACin, LOW);
  digitalWrite(ACout, LOW);
  digitalWrite(DCamp, LOW);
  // enable DC in and out
  digitalWrite(DCin, HIGH);
  digitalWrite(DCout, HIGH);
  clk_through();  // change relay configuration
  power_down();   // power down
}

void switch_DC_amp(){
  power_up();     // power up
  // disable AC in, out and DC out
  digitalWrite(ACin, LOW);
  digitalWrite(ACout, LOW);
  digitalWrite(DCout, LOW);
  // enable DC in and amp
  digitalWrite(DCin, HIGH);
  digitalWrite(DCamp, HIGH);
  clk_through();  // change relay configuration
  power_down();   // power down
}
