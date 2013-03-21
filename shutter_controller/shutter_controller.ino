/*
Shutter Control Driver v0.7 (13/11/12, A. Sanders)
For use with standard drive circuit.
ASCII controlled commands enabled.
*/

// Inputs
const int shtrCtrl = 2;
const int shtrMode = 4;
const int endTrig = 6;
const int extTrig = 7;
// Outputs
const int motorDirection = 12;
const int motorPWM = 3;
const int motorBrake = 9;

// Pin-to-Bit
// PORTD
const int shtrCtrl_bit = 2;//bit(2);
const int motorPWM_bit = 3;//bit(3);
const int shtrMode_bit = 4;//bit(4);
const int endTrig_bit = 6;//bit(6);
const int extTrig_bit = 7;//bit(7);
// PORTB
const int motorBrake_bit = 1;//bit(1);
const int motorDirection_bit = 4;//bit(4);

// Declarations
String inputString = "";
boolean stringComplete = false;

int shtr;
float current;
float V_supply = 18.0;
int V_hold = (1.0/V_supply)*255;

void setup(){
  pinMode(shtrCtrl, INPUT);
  pinMode(shtrMode, INPUT);
  pinMode(endTrig, INPUT);
  pinMode(extTrig, INPUT);
  pinMode(motorDirection, OUTPUT);
  pinMode(motorPWM, OUTPUT);
  pinMode(motorBrake, OUTPUT);
  
  Serial.begin(9600);
  inputString.reserve(200);
  
  initialiseMotor();
}

void loop(){
  /* Serial Commands and Communications */
  if (stringComplete) {
    if (inputString == "test") {
      Serial.println("Test Sequence Enabled");
    }
    if (inputString == "open") openShtr();
    if (inputString == "close") closeShtr();
    if (inputString == "flip") flipShtr();
    if (inputString == "trig") trigSeq();
    inputString = "";
    stringComplete = false;
  }
  
  /* Manual Controls */
  if (digitalRead(shtrMode) == LOW) trigSeq();
  else if (digitalRead(shtrCtrl) == LOW) flipShtr();
  delay(150);
}
