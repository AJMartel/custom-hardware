void switchAC(){  
  PORTD &= B01111111; // disable DC mode (7, 8 & 12)
  PORTB &= B11101110;
  PORTD |= B00010100; // enable AC mode (2 & 4)
}

void switchDC(){
  PORTD &= B11101011; // disable AC mode (2 & 4)
  PORTB &= B11101111; // disable DC amp mode (12)
  PORTD |= B10000000; // enable DC mode (7 & 8)
  PORTB |= B00000001;
}

void switchDCa(){
  PORTD &= B11101011; // disable AC mode (2 & 4)
  PORTB &= B11111110; // disable DC mode (8)
  PORTD |= B10000000; // enable DC amp mode (7 & 12)
  PORTB |= B00010000;
}

void reset(){
  PORTD &= B00000011; // disable AC mode (2 & 4)
  PORTB &= B00000000; // disable DC mode (8)
}

void switchACin(){  
  PORTD |= B00000100; // enable ACin (2)
}

void switchACout(){  
  PORTD |= B00010000; // enable ACout (4)
}

void switchDCin(){  
  PORTD |= B10000000; // enable DCin (7)
}

void switchDCout(){
  PORTB &= B11101111; // disable DCamp (12)
  PORTB |= B00000001; // enable DCout (8)
}

void switchDCamp(){ 
  PORTB &= B11111110; // disable DCout (8)
  PORTB |= B00010000; // enable DCamp (12)
}

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
