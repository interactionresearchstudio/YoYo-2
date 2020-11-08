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


void advanceAngle() {
  // Advance
  if (isIncreasing) currentAngle += 180 / numOfSections;
  else currentAngle -= 180 / numOfSections;

  // Change direction if we're at either end
  if (currentAngle > 180) {
    isIncreasing = false;
    currentAngle -= 2 * (180 / numOfSections);
  } else if (currentAngle < 0) {
    isIncreasing = true;
    currentAngle += 2 * (180 / numOfSections);
  }

  // Write to servo.
  servo.write(currentAngle);
  Serial.print("New angle: ");
  Serial.println(currentAngle);
}
