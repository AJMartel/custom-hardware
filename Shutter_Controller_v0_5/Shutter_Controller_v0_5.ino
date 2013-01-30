/*
Shutter Control Driver v0.5 (26/19/12, A. Sanders)
For use with FET-modified drive circuit.
*/

#define bit_get(p,m) ((p) & (m)) 
#define bit_set(p,m) ((p) |= (m)) 
#define bit_clear(p,m) ((p) &= ~(m)) 
#define bit_flip(p,m) ((p) ^= (m)) 
#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m)) 
#define BIT(x)	(0x01 << (x)) 
#define LONGBIT(x) ((unsigned long)0x00000001 << (x))

// Inputs
const int shtrCtrl = 2;
const int shtrMode = 4;
const int endTrig = 6;
const int extTrig = 7;
const int clk = 7;
// Outputs
const int motorDirection = 12;
const int motorPWM = 3;
const int motorBrake = 9;
const int clr = 6;
// Indicators
const int led = 13;

// Pin-to-Bit
// PORTD
const int shtrCtrl_bit = BIT(2);
const int motorPWM_bit = BIT(3);
const int shtrMode_bit = BIT(4);
const int clr_bit = BIT(6);
const int endTrig_bit = BIT(6);
const int extTrig_bit = BIT(7);
const int clk_bit = BIT(7);
// PORTB
const int motorBrake_bit = BIT(1);
const int motorDirection_bit = BIT(4);
const int led_bit = BIT(5);

int shtr;
float current;
float V_supply = 18.0;
int V_hold = (1.0/V_supply)*255;

void setup(){
  pinMode(shtrCtrl, INPUT);
  pinMode(shtrMode, INPUT);
  //pinMode(endTrig, INPUT);
  pinMode(extTrig, INPUT);
  //pinMode(clk, OUTPUT);
  pinMode(clr, OUTPUT);
  pinMode(motorDirection, OUTPUT);
  pinMode(motorPWM, OUTPUT);
  pinMode(motorBrake, OUTPUT);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  engagePower();
  initialiseMotor();
  closeShtr();
  openShtr();
  closeShtr();
}

void loop(){
  if (digitalRead(shtrMode) == LOW) {
    Serial.println("Trigger Mode Enabled");
    armShtr();
    delay(100);
    while (true) {
      // Power will be engaged on external trigger
      if (bit_get(PIND, extTrig_bit)){
        openShtr();
        holdShtr();
        Serial.println("Triggered");
      }
      //else if (bit_get(PIND, endTrig_bit)) break;
      else if (!bit_get(PIND, shtrMode_bit)) break;
      else if (!bit_get(PIND, shtrCtrl_bit)) break;
    }
    disarmShtr();
    Serial.println("Sequence Complete");
  }
  else if (digitalRead(shtrCtrl) == LOW) flipShtr();
  delay(150);
}

void engagePower(){
  digitalWrite(clr, HIGH);
  pinMode(clk, OUTPUT);
  digitalWrite(clk, LOW);
  delayMicroseconds(10);
  digitalWrite(clk, HIGH);
  delayMicroseconds(10);
  digitalWrite(clk, LOW);
  delay(100);
  pinMode(extTrig, INPUT);
}

void disengagePower(){
  pinMode(clr, OUTPUT);
  digitalWrite(clr, HIGH);
  delayMicroseconds(10);
  digitalWrite(clr, LOW);
  delayMicroseconds(10);
  digitalWrite(clr, HIGH);
  delay(100);
}

void initialiseMotor(){
  analogWrite(motorPWM, 255);
  digitalWrite(motorBrake, LOW);
  digitalWrite(motorDirection, LOW);
  analogWrite(motorPWM, 0);
}

void closeShtr(){
  analogWrite(motorPWM, 255);
  digitalWrite(motorDirection, LOW);
  delay(50);
  analogWrite(motorPWM,0);
  shtr = 0;
}

void openShtr(){
  analogWrite(motorPWM,255);
  digitalWrite(motorDirection, HIGH);
  delay(50);
  analogWrite(motorPWM, V_hold);
  shtr = 1;
}

void flipShtr(){
  analogWrite(motorPWM,255);
  bit_flip(PORTB, motorDirection_bit);
  delay(50);
  if (digitalRead(motorDirection) == HIGH) analogWrite(motorPWM, V_hold);
  else analogWrite(motorPWM, 0);
  shtr ^= 1;
  Serial.print("Shutter ");
  if (shtr == 0) Serial.println("Closed");
  else Serial.println("Open");
}

void armShtr(){
  closeShtr();
  delay(100);
  disengagePower();
  analogWrite(motorPWM,255);
  digitalWrite(motorDirection, HIGH);
}

void holdShtr(){
  // Once triggered maintain position
  delay(100); // Delay to allow complete opening
  analogWrite(motorPWM, V_hold);
  shtr = 1;
}

void disarmShtr(){
  engagePower();
  closeShtr();
}
