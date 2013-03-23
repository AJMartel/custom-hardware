/*
Multi-Shutter Control Driver v2 (04/01/13, A. Sanders)
For use with standard drive circuit.
Text string ASCII commands enabled.
*/

// No Inputs
// Outputs
const int motorA_dir = 12;
const int motorA_pwm = 3;
const int motorA_brake = 9;
const int motorB_dir = 13;
const int motorB_pwm = 11;
const int motorB_brake = 8;

// Pin-to-Bit
// PORTD
const int motorA_pwm_bit = 3;
// PORTB
const int motorB_brake_bit = 0;
const int motorA_brake_bit = 1;
const int motorB_pwm_bit = 3;
const int motorA_dir_bit = 4;
const int motorB_dir_bit = 5;

int shtrA, shtrB;
float current;
float V_supply = 5.0;
int V_hold = (1.0/V_supply)*255;
String inputString = "";
boolean stringComplete = false;

void setup(){
  pinMode(motorA_dir, OUTPUT);
  pinMode(motorA_pwm, OUTPUT);
  pinMode(motorA_brake, OUTPUT);
  pinMode(motorB_dir, OUTPUT);
  pinMode(motorB_pwm, OUTPUT);
  pinMode(motorB_brake, OUTPUT);
  Serial.begin(9600);
  initialiseMotors();
  Serial.println("Shutters Initialised");
  inputString.reserve(200);
}

void loop(){
  /* Serial Commands and Communications */
  if (stringComplete) {
    //Serial.println(inputString);
    if (inputString == "test") {
      Serial.println("Test Sequence Enabled");
      closeShtrA();
      openShtrA();
      closeShtrA();
      closeShtrB();
      openShtrB();
      closeShtrB();
    }
    if (inputString == "openA") openShtrA();
    if (inputString == "closeA") closeShtrA();
    if (inputString == "flipA") flipShtrA();
    if (inputString == "openB") openShtrB();
    if (inputString == "closeB") closeShtrB();
    if (inputString == "flipB") flipShtrB();
    if (inputString == "open") openShtrs();
    if (inputString == "close") closeShtrs();
    if (inputString == "flip") flipShtrs();
    if (inputString == "statusA") statusShtrA();
    if (inputString == "statusB") statusShtrB();
    if (inputString == "status") statusShtrs();
    inputString = "";
    stringComplete = false;
  }
  //delay(150);
}
