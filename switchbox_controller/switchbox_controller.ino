/*
Circuit Switch Control Driver v0 (08/01/13, A. Sanders)
For use with tip experiment electronics switch box.
*/

// Inputs
// Outputs
const int ACin = 2;
const int ACout = 4;
const int DCin = 7;
const int DCout = 8;
const int DCamp = 12;

// Pin-to-Bit
// PORTD
const int ACin_bit = 2;
const int ACout_bit = 4;
const int DCin_bit = 7;
// PORTB
const int DCout_bit = 0;
const int DCamp_bit = 4;

// Declarations
String inputString = "";
boolean stringComplete = false;

void setup(){
  /*pinMode(ACin, OUTPUT);
  pinMode(ACout, OUTPUT);
  pinMode(DCin, OUTPUT);
  pinMode(DCout, OUTPUT);
  pinMode(DCamp, OUTPUT);
  digitalWrite(ACin, LOW);
  digitalWrite(ACout, LOW);
  digitalWrite(DCin, LOW);
  digitalWrite(DCout, LOW);
  digitalWrite(DCamp, LOW);*/
  
  DDRD |= B10010100;
  DDRB |= B00010001;
  PORTD &= B01101011;
  PORTB &= B11101110;
  
  Serial.begin(9600);
  inputString.reserve(200);
}

void loop(){
  /* Serial Commands and Communications */
  if (stringComplete) {
    if (inputString == "test") {
      Serial.println("Test Sequence Enabled");
    }
    if (inputString == "switchAC") switchAC();
    if (inputString == "switchDC") switchDC();
    if (inputString == "switchDCamp") switchDCamp();
    if (inputString == "reset") reset();
    if (inputString == "config?") config_status();
    if (inputString == "enableACin") enableACin();
    if (inputString == "enableACout") enableACout();
    if (inputString == "enableDCin") enableDCin();
    if (inputString == "enableDCout") enableDCout();
    if (inputString == "enableDCamp") enableDCamp();
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent(){
  /* Serial Communications */
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n'){
      stringComplete = true;
      break;}
    inputString += inChar;
    //if (inChar == '\n') stringComplete = true;
  }
}

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

void switchDCamp(){
  PORTD &= B11101011; // disable AC mode (2 & 4)
  PORTB &= B11111110; // disable DC mode (8)
  PORTD |= B10000000; // enable DC amp mode (7 & 12)
  PORTB |= B00010000;
}

void reset(){
  PORTD &= B00000011; // disable AC mode (2 & 4)
  PORTB &= B00000000; // disable DC mode (8)
}

void enableACin(){  
  PORTD |= B00000100; // enable ACin (2)
}

void enableACout(){  
  PORTD |= B00010000; // enable ACout (4)
}

void enableDCin(){  
  PORTD |= B10000000; // enable DCin (7)
}

void enableDCout(){
  PORTB &= B11101111; // disable DCamp (12)
  PORTB |= B00000001; // enable DCout (8)
}

void enableDCamp(){ 
  PORTB &= B11111110; // disbale DCout (8)
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
