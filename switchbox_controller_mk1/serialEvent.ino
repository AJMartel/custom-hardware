/* requires delcaration of:
String inputString = "";
boolean stringComplete = false;
and line:
Serial.begin(9600);
inputString.reserve(200);
in setup() */

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
