void setupServo() {
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  servo.setPeriodHertz(50);    // standard 50 hz servo
  servo.attach(servoPin, 150, 2500); // attaches the servo on pin 18 to the servo object
  servo.write(angles[numOfSections-1]);
  delay(1000);
  servo.write(angles[0]);
  delay(500);
}

void setAngle(byte angle) {
  Serial.print("Moved servo to ");
  Serial.println(angle);
  servo.write(angle);
}

void setPosition(byte pos) {
  Serial.print("Moved servo to ");
  Serial.println(angles[pos]);
  servo.write(angles[pos]);
  currAngleArrayPosition = pos;
}

byte getPosition() {
  return currAngleArrayPosition;
}

byte getAngle() {
  return angles[currAngleArrayPosition];
}

void generateAngles() {
  for (byte i = 0; i < numOfSections; i++) {
    angles[i] = (i * (180 / (numOfSections))) + ((180 / numOfSections) / 2);
  }
  angles[0] = angles[0] - 2;
  angles[numOfSections - 1] = angles[numOfSections - 1] + 2;
}

void advanceAngle() {
  // Advance
  if (isIncreasing) currAngleArrayPosition++;
  else currAngleArrayPosition--;

  // Change direction if we're at either end
  if (currAngleArrayPosition == numOfSections) {
    isIncreasing = false;
    currAngleArrayPosition = currAngleArrayPosition - 2 ;
  } else if (currAngleArrayPosition < 0) {
    isIncreasing = true;
    currAngleArrayPosition = currAngleArrayPosition + 2;
  }

  Serial.print("New angle: ");
  Serial.println(angles[currAngleArrayPosition]);
}
