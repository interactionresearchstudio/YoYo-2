void servoHandler() {
  if (millis() - prevServoMillis > SERVO_REFRESH_RATE) {
    prevServoMillis = millis();
    for (byte i = 0; i < NUMSERVOS; i++) {
      if (currAngle[i] != prevAngle[i]) {
        updateServo(i, currAngle[i]);
        prevAngle[i] = currAngle[i];
      }
    }
  }
}

byte getServoAngle(byte servoNum) {
  if (servoNum < NUMSERVOS) {
    return currAngle[servoNum];
  } else {
    return 0;
  }
}

void setServoAngle(byte servoNum, int angle) {
  if (servoNum < NUMSERVOS) {
    currAngle[servoNum] = angle;
  }
}

void updateServo(byte servoNum, int angle) {
  servo[servoNum].write(angle);
}

void setupServo() {
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  for (byte i = 0; i < NUMSERVOS; i++) {
    servo[i].setPeriodHertz(50);    // standard 50 hz servo
    servo[i].attach(servoPin[i], 150, 2500); // attaches the servo on pin 18 to the servo object
    servo[i].write(0);
  }

}
