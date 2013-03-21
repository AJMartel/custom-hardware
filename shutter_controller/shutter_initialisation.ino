void initialiseMotor(){
  analogWrite(motorPWM, 255);
  digitalWrite(motorBrake, LOW);
  digitalWrite(motorDirection, LOW);
  analogWrite(motorPWM, 0);
  closeShtr();
  openShtr();
  closeShtr();
}
