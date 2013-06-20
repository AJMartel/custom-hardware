/* requires delcaration of:
String inputString = "";
boolean stringComplete = false;
and line in setup():
Serial.begin(9600);
inputString.reserve(200);

to use place the following in the loop:
if (stringComplete) {
    if (inputString == "cmd1") do_something;
    else if (inputString == "cmd2") do_something_else;
    }
    inputString = "";
    stringComplete = false; */

#define COMMS_REQUEST "A"

void serialEvent(){
  /* Serial Communications */
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n'){
      stringComplete = true;
      break;}
    inputString += inChar;
  }
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.write(COMMS_REQUEST);   // send an initial string
    delay(300);
  }
}
