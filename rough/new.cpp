
void loop() { 

  int sensorValue21 = digitalRead(IR_SENSOR_PIN2);

  if (sensorValue21 == pattern[0]) {
    if (check()) {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("pattern valid");
      myServo1.write(90);
    delay(500);
    myServo2.write(90);
    myServo4.write(90);
    myServo3.write(0);
    delay(1000); // Wait for 1 second
    myServo3.write(90);
  // delay(3000); // Wait for 5 seconds
  delay(1000);
    int sensorValue22 = digitalRead(IR_SENSOR_PIN);
    while(sensorValue22==LOW)
    {
      int touch2=(touchRead(4)<40);
      if(touch2)
      {
        myServo2.write(0);
        delay(500); // Wait for 0.5 seconds
        myServo1.write(0);
       myServo4.write(0);
        Serial.println("Window break2");
        digitalWrite(LED_PIN, HIGH); // Turn on the LED
        delay(2000); // Wait for 1 second (1000 milliseconds)
        digitalWrite(LED_PIN, LOW);
        break;
      }
      digitalWrite(TRIGGER_PIN, LOW);
      delayMicroseconds(2);
      digitalWrite(TRIGGER_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIGGER_PIN, LOW);
      long duration = pulseIn(ECHO_PIN, HIGH);
      float distance = duration * 0.034 / 2; 
      if (distance < 23) {
        myServo1.write(90);
        delay(500);
        myServo2.write(0);
        myServo3.write(90);
       myServo4.write(0);
        delay(500); // Wait for 0.5 seconds
        myServo1.write(0);
        digitalWrite(LED_PIN, HIGH); // Turn off the LED
        delay(2000); // Wait for 1 second (1000 milliseconds)
        digitalWrite(LED_PIN, LOW);
        break;
    }
      delay(1000);
      sensorValue22 = digitalRead(IR_SENSOR_PIN);
    }
    for(int x=0;x<6;x++)
    {
      int touch3=(touchRead(4)<40);
      if(touch3)
      {
        myServo2.write(0);
        delay(500); // Wait for 0.5 seconds
        myServo1.write(0);
       myServo4.write(0);
        Serial.println("Window break2");
        digitalWrite(LED_PIN, HIGH); // Turn on the LED
        delay(2000); // Wait for 1 second (1000 milliseconds)
        digitalWrite(LED_PIN, LOW);
      }
      digitalWrite(TRIGGER_PIN, LOW);
      delayMicroseconds(2);
      digitalWrite(TRIGGER_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIGGER_PIN, LOW);
      long duration = pulseIn(ECHO_PIN, HIGH);
      float distance = duration * 0.034 / 2; 
      if (distance < 23) {
        myServo1.write(90);
        delay(500);
        myServo2.write(0);
        myServo3.write(90);
       myServo4.write(0);
        delay(500); // Wait for 0.5 seconds
        myServo1.write(0);
        digitalWrite(LED_PIN, HIGH); // Turn off the LED
        delay(2000); // Wait for 1 second (1000 milliseconds)
        digitalWrite(LED_PIN, LOW);
        break;
    }
      delay(1000);
    }
    delay(1000);
    myServo2.write(0);
     myServo4.write(0);
    delay(500); // Wait for 0.5 seconds
    myServo1.write(0);
    
    } 
    else {
      Serial.println("pattern Invalid");
    }
  } else {
    Serial.println("pattern Invalid");
  }
  
    delay(1000);
}


bool check() {
  Serial.println("pattern");
  delay(patternDelay);
 
  for (int i = 1; i < patternLength; i++) {
    int sensorValue23 = digitalRead(IR_SENSOR_PIN2);
    if (sensorValue23 != pattern[i]) {
      return false;
    }
    Serial.println(i);
    delay(patternDelay);
  }
  return true;
}
