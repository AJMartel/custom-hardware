/*
Multi-Shutter Control Driver v1 (04/01/13, A. Sanders)
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

void serialEvent() {
  /* Serial Communications */
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
      break;
    }
    inputString += inChar;
    //if (inChar == '\n') stringComplete = true;
  }
}

void initialiseMotors(){
  analogWrite(motorA_pwm, 255);
  digitalWrite(motorA_brake, LOW);
  digitalWrite(motorA_dir, LOW);
  analogWrite(motorA_pwm, 0);
  analogWrite(motorB_pwm, 255);
  digitalWrite(motorB_brake, LOW);
  digitalWrite(motorB_dir, LOW);
  analogWrite(motorB_pwm, 0);
  closeShtrA(); closeShtrB();
}

void closeShtrA(){
  analogWrite(motorA_pwm, 255);
  digitalWrite(motorA_dir, LOW);
  delay(50);
  analogWrite(motorA_pwm, V_hold);
  delay(1000);
  analogWrite(motorA_pwm, 0);
  shtrA = 0;
}

void openShtrA(){
  analogWrite(motorA_pwm,255);
  digitalWrite(motorA_dir, HIGH);
  delay(50);
  analogWrite(motorA_pwm, V_hold);
  delay(50);
  analogWrite(motorA_pwm, 0);
  shtrA = 1;
}

void flipShtrA(){
  analogWrite(motorA_pwm,255);
  PORTB ^= bit(motorA_dir_bit);
  delay(50);
  if (digitalRead(motorA_dir) == HIGH) {
    analogWrite(motorA_pwm, V_hold);
    delay(500);
    analogWrite(motorA_pwm, 0);
  }
  else analogWrite(motorA_pwm, 0);
  shtrA ^= 1;
}

void statusShtrA(){
  Serial.print("Shutter A ");
  if (shtrA == 0) Serial.println("Closed");
  else Serial.println("Open");
}

void closeShtrB(){
  analogWrite(motorB_pwm, 255);
  digitalWrite(motorB_dir, LOW);
  delay(50);
  analogWrite(motorB_pwm, 0);
  shtrB = 0;
}

void openShtrB(){
  analogWrite(motorB_pwm,255);
  digitalWrite(motorB_dir, HIGH);
  delay(50);
  analogWrite(motorB_pwm, V_hold);
  delay(50);
  analogWrite(motorB_pwm, 0);
  shtrB = 1;
}

void flipShtrB(){
  analogWrite(motorB_pwm,255);
  PORTB ^= bit(motorB_dir_bit);
  delay(50);
  if (digitalRead(motorB_dir) == HIGH) {
    analogWrite(motorB_pwm, V_hold);
    delay(500);
    analogWrite(motorB_pwm, 0);
  }
  else analogWrite(motorB_pwm, 0);
  shtrB ^= 1;
}

void statusShtrB(){
  Serial.print("Shutter B ");
  if (shtrB == 0) Serial.println("Closed");
  else Serial.println("Open");
}

void closeShtrs(){
  analogWrite(motorA_pwm, 255);
  analogWrite(motorB_pwm, 255);
  digitalWrite(motorA_dir, LOW);
  digitalWrite(motorB_dir, LOW);
  delay(50);
  analogWrite(motorA_pwm, 0);
  analogWrite(motorB_pwm, 0);
  shtrA = 0; shtrB = 0;
}

void openShtrs(){
  analogWrite(motorA_pwm,255);
  analogWrite(motorB_pwm,255);
  digitalWrite(motorA_dir, HIGH);
  digitalWrite(motorB_dir, HIGH);
  delay(50);
  analogWrite(motorA_pwm, V_hold);
  analogWrite(motorB_pwm, V_hold);
  delay(50);
  analogWrite(motorA_pwm, 0);
  analogWrite(motorB_pwm, 0);
  shtrA = 1; shtrB = 1;
}

void flipShtrs(){
  analogWrite(motorA_pwm,255);
  analogWrite(motorB_pwm,255);
  PORTB ^= bit(motorA_dir_bit);
  PORTB ^= bit(motorB_dir_bit);
  delay(50);
  if (digitalRead(motorA_dir) == HIGH) {
    analogWrite(motorA_pwm, V_hold);
    delay(500);
    analogWrite(motorA_pwm, 0);
  }
  else analogWrite(motorA_pwm, 0);
  shtrA ^= 1;
  if (digitalRead(motorB_dir) == HIGH) {
    analogWrite(motorB_pwm, V_hold);
    delay(500);
    analogWrite(motorB_pwm, 0);
  }
  else analogWrite(motorB_pwm, 0);
  shtrB ^= 1;
}

void statusShtrs(){
  Serial.print("Shutter A ");
  if (shtrA == 0) Serial.print("Closed,");
  else Serial.print("Open, ");
  Serial.print("Shutter B ");
  if (shtrB == 0) Serial.println("Closed");
  else Serial.println("Open");
}
