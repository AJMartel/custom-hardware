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
  initialiseMotor();
}

void loop(){
  /* Serial Communications */
  while (Serial.available() > 0) {
    int inByte = Serial.read();
    switch (inByte) {
    case 'o'://'shtrOpen':
      openShtr();
      break;
    case 'c'://'shtrClose':
      closeShtr();
      break;
    case 'f'://'shtrFlip':
      flipShtr();
      break;
    case 't'://'trigSeq':
      //int exp_time = Serial.parseInt();
      //int t_range = 256 * (exp_time + 9.2);
      //trigSeq(t_range);   
      trigSeq();
      break;
    }
  }
  
  /* Manual Controls */
  if (digitalRead(shtrMode) == LOW) trigSeq();
  else if (digitalRead(shtrCtrl) == LOW) flipShtr();
  delay(150);
}

void trigSeq(){
  /* Trigger Mode */
  Serial.println("Trigger Mode Enabled");
  closeShtr();
  
  /* Get exposure parameters */
  int exp_time = Serial.parseInt();
  int t_range = 256 * (exp_time + 9.2);
  Serial.print(exp_time);
  Serial.print(", ");
  Serial.println(t_range/1000);
  
  /* Trigger Loop */
  int trig = 0;
  while (!trig) {
    
    if (Serial.available() > 0) {
      Serial.println("Exit Requested");
      int inByte = Serial.read();
      switch (inByte) {
      case 'e':
        trig = 1;
      }
    }
    
    if (bitRead(PIND, extTrig_bit)){
      analogWrite(motorPWM,255);
      bitSet(PORTB, motorDirection_bit);
      //delayMicroseconds(t_range);
      delay(t_range/1000);
      bitClear(PORTB, motorDirection_bit);
      //delay(50);
      //analogWrite(motorPWM, V_hold);
      delay(50);
      analogWrite(motorPWM, 0);
      Serial.println("Triggered");
      break;
    }
    //else if (bitRead(PIND, endTrig_bit)) break;
    else if (!bitRead(PIND, shtrCtrl_bit)) break;
  }
  closeShtr();
  Serial.println("Sequence Complete");
}

void chopShtr(float f){
  float t = 1/f;
  analogWrite(motorPWM, 255);
  int i = 4;
  while(i--){
    bitSet(PORTB, motorDirection_bit);
    delay(t/2);
    bitClear(PORTB, motorDirection_bit);
    delay(t/2);
    Serial.println(i);
  }
  analogWrite(motorPWM, 0);
}

void initialiseMotor(){
  analogWrite(motorPWM, 255);
  digitalWrite(motorBrake, LOW);
  digitalWrite(motorDirection, LOW);
  analogWrite(motorPWM, 0);
  closeShtr();
  openShtr();
  closeShtr();
}

void closeShtr(){
  analogWrite(motorPWM, 255);
  digitalWrite(motorDirection, LOW);
  delay(50);
  analogWrite(motorPWM, 0);
  shtr = 0;
}

void openShtr(){
  analogWrite(motorPWM,255);
  digitalWrite(motorDirection, HIGH);
  delay(50);
  analogWrite(motorPWM, V_hold);
  delay(50);
  analogWrite(motorPWM, 0);
  shtr = 1;
}

void flipShtr(){
  analogWrite(motorPWM,255);
  PORTB ^= bit(motorDirection_bit);
  delay(50);
  if (digitalRead(motorDirection) == HIGH) {
    analogWrite(motorPWM, V_hold);
    delay(500);
    analogWrite(motorPWM, 0);
  }
  else analogWrite(motorPWM, 0);
  shtr ^= 1;
  Serial.print("Shutter ");
  if (shtr == 0) Serial.println("Closed");
  else Serial.println("Open");
}
