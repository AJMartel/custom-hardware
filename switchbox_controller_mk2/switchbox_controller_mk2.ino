/*
Circuit Switch Control Driver v0 (08/01/13, A. Sanders)
For use with tip experiment electronics switch box.
*/

// Outputs
const int ACin = 2;
const int DCin = 3;
const int ACout = 4;
const int clk = 5;
const int DCout = 6;
const int DCamp = 7;
const int power_clk = 8;
const int power = 9;

// Pin-to-Bit
// PORTD
const int ACin_bit = 2;
const int DCin_bit = 3;
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
  
  DDRD |= B11111100;
  DDRB |= B00000011;
  PORTD &= B00000011;
  PORTB &= B11111100;
  
  Serial.begin(9600);
  inputString.reserve(200);
}

void loop(){
  /* Serial Commands and Communications */
  if (stringComplete) {
    // test commands
    if (inputString == "id?") Serial.println("switchbox controller");
    if (inputString == "test") {
      Serial.println("Test Sequence Enabled");}
    // switch routing modes
    if (inputString == "switch_ac") switch_AC();
    if (inputString == "switch_dc") switch_DC();
    if (inputString == "switch_dc_amp") switch_DC_amp();
    // configuration commands
    if (inputString == "reset") reset();
    if (inputString == "config?") config_status();
    // more commands
    if (inputString == "power_up") power_up();
    if (inputString == "power_down") power_down();
    if (inputString == "clk") clk_through();
    // switch individual relays
    if (inputString == "switch_ac_in") switch_ac_in();
    if (inputString == "switch_ac_out") switch_ac_out();
    if (inputString == "switch_dc_in") switch_dc_in();
    if (inputString == "switch_dc_out") switch_dc_out();
    if (inputString == "switch_dcamp") switch_dcamp();
    // reset command string
    inputString = "";
    stringComplete = false;
  }
}
