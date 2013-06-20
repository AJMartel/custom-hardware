/* requires delcaration of:
String inputString = "";
boolean stringComplete = false;
and line:
Serial.begin(9600);
inputString.reserve(200);
in setup() */

/*
to use place:

if (stringComplete) {
    if (inputString == "cmd1") do_something;
    else if (inputString == "cmd2") do_something_else;
    }
    inputString = "";
    stringComplete = false;

in the loop.
*/

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
