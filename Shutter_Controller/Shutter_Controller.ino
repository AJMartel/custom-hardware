/*
Shutter Control Driver, v0.4
v0 - 31/08/12 (A. Sanders)
v0.1 - 04/09/12 (A. Sanders)
v0.2 - 21/09/12 (A. Sanders)
v0.3 - 24/09/12 (A. Sanders)
Fully operational shutter with 0.5 ms latency and 20-30 us opening.
v0.4 - 25/09/12 (A. Sanders)
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
// Outputs
const int motorDirection = 12;
const int motorPWM = 3;
const int motorBrake = 9;
const int gate = 5;
// Indicators
const int led = 13;

// Pin-to-Bit
// PORTD
const int shtrCtrl_bit = BIT(2);
const int motorPWM_bit = BIT(3);
const int shtrMode_bit = BIT(4);
const int gate_bit = BIT(5);
const int endTrig_bit = BIT(6);
const int extTrig_bit = BIT(7);
// PORTB
const int motorBrake_bit = BIT(1);
const int motorDirection_bit = BIT(4);
const int led_bit = BIT(5);

int shtr;
float current;
int V_supply = 15.0;
int V_hold = (1.0/V_supply)*255;

void setup(){
  // Set I/O
  pinMode(shtrCtrl, INPUT);
  pinMode(shtrMode, INPUT);
  pinMode(endTrig, INPUT);
  pinMode(extTrig, INPUT);
  pinMode(motorDirection, OUTPUT);
  pinMode(motorPWM, OUTPUT);
  pinMode(motorBrake, OUTPUT);
  pinMode(gate, OUTPUT);
  pinMode(led, OUTPUT);
  // Setup communications
  Serial.begin(9600);
  // Setup motor
  analogWrite(motorPWM, 255);
  //TCCR2A = _BV(COM2A0) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  //TCCR2B = _BV(WGM22) | _BV(CS22);
  //OCR2A = 50;
  //OCR2B = 50;
  //digitalWrite(motorPWM,HIGH);
  digitalWrite(motorBrake, LOW);
  digitalWrite(gate, HIGH);
  // Close shutter
  //shtr = 1;
  closeShtr;
  openShtr;
  closeShtr;
}

void loop(){
  if (!bit_get(PIND, shtrMode_bit)) {
    Serial.println("Trigger Mode Enabled");
    // Close shutter and wait for trigger
    closeShtr();
    delay(100);
    // Check for trigger
    while (true) {
      if (bit_get(PIND, extTrig_bit)) openShtr();
      //else if (bit_get(PIND, endTrig_bit)) break;
      // Manual Close Commands
      else if (!bit_get(PIND, shtrMode_bit)) break;
      else if (!bit_get(PIND, shtrCtrl_bit)) break;
    }
    Serial.println("Sequence Complete");
    closeShtr();
  }
  else if (!bit_get(PIND, shtrCtrl_bit)) flipShtr();
  delay(150);
}

void closeShtr(){
  analogWrite(motorPWM, 255);
  //bit_set(PORTD, motorPWM_bit);
  //OCR2A = 255;
  bit_clear(PORTB, motorDirection_bit);
  delay(50);
  analogWrite(motorPWM,0);
  //bit_clear(PORTD, motorPWM_bit);
  //OCR2A = 0;
}

void openShtr(){
  analogWrite(motorPWM,255);
  //bit_set(PORTD, motorPWM_bit);
  //OCR2A = 255;
  bit_set(PORTB, motorDirection_bit);
  delay(50);
  analogWrite(motorPWM, V_hold);
  //bit_clear(PORTD, motorPWM_bit);
  //OCR2A = V_hold;
}

void flipShtr(){
  analogWrite(motorPWM,255);
  //bit_set(PORTD, motorPWM_bit);
  //OCR2A = 255;
  bit_flip(PORTB, motorDirection_bit);
  delay(50);
  if (bit_get(PORTB, motorDirection_bit)) {
    analogWrite(motorPWM, V_hold);
    //bit_clear(PORTD, motorPWM_bit);
    //OCR2A = V_hold;
  }
  else {
    analogWrite(motorPWM, 0);
    //bit_clear(PORTD, motorPWM_bit);
    //OCR2A = 0;
  }
}

void armShtr(){
  // Close shutter
  analogWrite(motorPWM, 255);
  bit_clear(PORTB, motorDirection_bit);
  delay(50);
  // Disengage gate to remove PWM power
  bit_clear(PORTx, gate_bit);
  // Charge shutter in opening direction
  bit_set(PORTB, motorDirection_bit);
}

void holdShtr(){
  // Once triggered maintain position
  // Delay to allow complete opening
  delay(50);
  // Reduce PWM to holding voltage
  analogWrite(motorPWM, V_hold);
}

void disarmShtr(){
  // Close shutter and turn off current
  analogWrite(motorPWM, 255);
  bit_clear(PORTB, motorDirection_bit);
  delay(50);
  analogWrite(motorPWM, 0);
  // Engage gate
  bit_set(PORTx, gate_bit);
}
  
