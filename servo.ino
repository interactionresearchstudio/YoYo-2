void setupServo() {
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
    servo.setPeriodHertz(50);    // standard 50 hz servo
    servo.attach(servoPin, 150, 2500); // attaches the servo on pin 18 to the servo object
    servo.write(0);

}

void setAngle(byte angle){
  servo.write(angle);
  currentAngle = angle;
}

byte getAngle(){
  return currentAngle;
}

void generateAngles(){
  for(byte i = 0; i < numOfSections; i++){
    angles[i] = (i*(180/(numOfSections)))+((180/numOfSections)/2);
  }
}

void advanceAngle() {
  // Advance
   if (isIncreasing) currAngleArrayPosition++;
  else currAngleArrayPosition--;
  
  // Change direction if we're at either end
  if (currAngleArrayPosition == numOfSections) {
    isIncreasing = false;
    currAngleArrayPosition = currAngleArrayPosition-2 ;
  } else if (currAngleArrayPosition < 0) {
    isIncreasing = true;
    currAngleArrayPosition = currAngleArrayPosition+2;
  }

  currentAngle = angles[currAngleArrayPosition];
  Serial.print("New angle: ");
  Serial.println(angles[currAngleArrayPosition]);
}
