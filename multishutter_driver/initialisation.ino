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
