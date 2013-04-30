/*
Circuit Switch Control Driver v0 (08/01/13, A. Sanders)
For use with tip experiment electronics switch box.
*/

// Outputs
const int ACin = 2;
const int DCin = 3
const int ACout = 4;
const int clk = 5;
const int DCout = 6;
const int DCamp = 7;
const int power_clk = 8;
const int power = 9;

// Pin-to-Bit
// PORTD
const int ACin_bit = 2;
const int DCin_bit = 3
const int ACout_bit = 4;
const int clk_bit = 5;
const int DCout_bit = 6;
const int DCamp_bit = 7;
// PORTB
const int power_clk_bit = 0;
const int power_bit = 1;

// Declarations
String inputString = "";
boolean stringComplete = false;

void setup(){
  // pin mode setup
  pinMode(ACin, OUTPUT);
  pinMode(ACout, OUTPUT);
  pinMode(DCin, OUTPUT);
  pinMode(DCout, OUTPUT);
  pinMode(DCamp, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(power, OUTPUT);
  pinMode(power_clk, OUTPUT);
  // initial configuration - all relays off
  digitalWrite(ACin, LOW);
  digitalWrite(ACout, LOW);
  digitalWrite(DCin, LOW);
  digitalWrite(DCout, LOW);
  digitalWrite(DCamp, LOW);
  digitalWrite(clk, LOW);
  digitalWrite(power, LOW);
  digitalWrite(power_clk, LOW);
  
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
    if (inputString == "id?") {
      Serial.println("switchbox controller");}
    if (inputString == "test") {
      Serial.println("Test Sequence Enabled");}
    if (inputString == "switchAC") switchAC();
    if (inputString == "switchDC") switchDC();
    if (inputString == "switchDCa") switchDCa();
    if (inputString == "reset") reset();
    if (inputString == "config?") config_status();
    if (inputString == "switchACin") switchACin();
    if (inputString == "switchACout") switchACout();
    if (inputString == "switchDCin") switchDCin();
    if (inputString == "switchDCout") switchDCout();
    if (inputString == "switchDCamp") switchDCamp();
    inputString = "";
    stringComplete = false;
  }
}
