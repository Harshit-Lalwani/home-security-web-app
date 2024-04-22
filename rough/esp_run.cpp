#include <ESP32Servo.h>
// #include <Servo.h>
#include <WiFi.h>
#include <HTTPClient.h>


const int servoPin1 = 12; //lock
const int servoPin2 = 13; //gate2
const int servoPin3 = 14; //window
const int servoPin4 = 19; //sliding
const int IR_SENSOR_PIN = 26;// gateir
const int IR_SENSOR_PIN2 = 21;//locking
int LED_PIN = 2; //buzzer
#define TRIGGER_PIN 18 // ultrasonic
#define ECHO_PIN 5     //
Servo myServo1;
Servo myServo2;
Servo myServo3;
Servo myServo4;
const int patternLength = 5;
const int patternDelay = 200;
int pattern[15] = { LOW, HIGH,LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH, HIGH, HIGH, HIGH,HIGH, LOW };
const int IR_SENSOR_PIN2 = 21;//locking 
const char* ssid = "Bakul ka baghula";
const char* password = "efjxu789";
const char* host = "api.thingspeak.com";
const char* WriteKey = "9Y1GL8LC5SQJZKHW";

class SendData {
public:
    void setup_wifi() {
        delay(10);
        // Connect to WiFi network
        Serial.println();
        Serial.println();
        Serial.print("Connecting to ");
        Serial.println(ssid);

        WiFi.begin(ssid, password);

        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.println("");
        Serial.println("WiFi connected");
    }

    void send_http(float values[8]) {
        if (WiFi.status() == WL_CONNECTED) {
            HTTPClient http;

            String url = "http://";
            url += host;
            url += "/update?api_key=";
            url += WriteKey;

            for (int i = 0; i < 8; i++) {
                url += "&field";
                url += String(i + 1);
                url += "=";
                url += String(values[i]);
            }

            http.begin(url);
            int httpCode = http.GET();

            if (httpCode > 0) {
                String payload = http.getString();
                Serial.println(payload);
            } else {
                Serial.println("HTTP request failed.");
            }
            http.end();
        }
    }
};

float getDistance() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;
  return distance;
}

void handleTouch(int touch) {
    while(touch) {
        Serial.println("Window break");
        myServo2.write(0);
        delay(500); // Wait for 0.5 seconds
        myServo1.write(0);
        digitalWrite(LED_PIN, HIGH); // Turn on the LED
        delay(2000); // Wait for 1 second (1000 milliseconds)
        digitalWrite(LED_PIN, LOW);
        touch=(touchRead(4)<40);
    }
}

void handleDistance(float distance, int threshold) {
    while (distance < threshold) {
        myServo1.write(90);
        delay(500);
        myServo2.write(0);
        myServo3.write(90);
        delay(500); // Wait for 0.5 seconds
        myServo1.write(0);
        digitalWrite(LED_PIN, HIGH); // Turn off the LED
        delay(2000); // Wait for 1 second (1000 milliseconds)
        digitalWrite(LED_PIN, LOW);
        distance = getDistance();
    }
}

void handleSensorValue() {
    Serial.println("lock break3");
    myServo1.write(90);
    delay(500);
    myServo2.write(0);
    myServo3.write(90);
    delay(500); // Wait for 0.5 seconds
    myServo1.write(0);
    digitalWrite(LED_PIN, HIGH); // Turn off the LED
    delay(2000); // Wait for 1 second (1000 milliseconds)
    digitalWrite(LED_PIN, LOW);
}

void handleSensorValue2(int sensorValue2) {
    while(sensorValue2==LOW) {
        int touch2=(touchRead(4)<40);
        if(touch2) {
            myServo2.write(0);
            delay(500); // Wait for 0.5 seconds
            myServo1.write(0);
            Serial.println("Window break2");
            digitalWrite(LED_PIN, HIGH); // Turn on the LED
            delay(2000); // Wait for 1 second (1000 milliseconds)
            digitalWrite(LED_PIN, LOW);
            break;
        }
        float distance = getDistance();
        handleDistance(distance, 23);
        delay(1000);
        sensorValue2 = digitalRead(IR_SENSOR_PIN);
    }
}

void handleTouch3(int touch3) {
    if(touch3) {
        myServo2.write(0);
        delay(500); // Wait for 0.5 seconds
        myServo1.write(0);
        Serial.println("Window break2");
        digitalWrite(LED_PIN, HIGH); // Turn on the LED
        delay(2000); // Wait for 1 second (1000 milliseconds)
        digitalWrite(LED_PIN, LOW);
    }
}

void setup() {
  Serial.begin(115200);
  myServo1.attach(servoPin1);
  myServo2.attach(servoPin2); 
  myServo3.attach(servoPin3);
  myServo4.attach(servoPin4);
  pinMode(IR_SENSOR_PIN, INPUT);
  pinMode(IR_SENSOR_PIN2, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  myServo2.write(0);
  delay(500);
  myServo1.write(0);
  myServo3.write(90);
  myServo4.write(0);
  digitalWrite(LED_PIN, LOW);
  SendData sender;
  sender.setup_wifi();
}

void collectData(float data[8]) {
    data[0]=  0;
    data[1] = detDistance();
    data[2] = touchRead(4);
    data[3]  = DigitalRead(IR_SENSOR_PIN);
    data[4]  =0;
    data[5] = DigitalRead(IR_SENSOR_PIN2);
    data[6] = 0;
    data[7] = 0;
}

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

    
  delay(2000);
  int touch=(touchRead(4)<40);
  if(touch)
  {
    handleTouch(touch);
  }

  handleDistance(getDistance(), 23);

  int sensorValue = digitalRead(IR_SENSOR_PIN);
  if(sensorValue == HIGH)
  {
    handleSensorValue();
  }
  else
  {
    myServo1.write(90);
    delay(500);
    myServo2.write(90);
    myServo3.write(0);
    delay(1000); // Wait for 1 second
    myServo3.write(90);
    // delay(3000); // Wait for 5 seconds
    delay(2000);
    int sensorValue2 = digitalRead(IR_SENSOR_PIN);
    handleSensorValue2(sensorValue2);

    for(int x=0;x<6;x++)
    {
      int touch3=(touchRead(4)<40);
      handleTouch3(touch3);

      float distance = getDistance();
      if (distance < 23) {
        handleDistance(distance, 23);
        break;
    }
      delay(1000);
    }

    delay(1000);

    myServo2.write(0);
    delay(500); // Wait for 0.5 seconds
    myServo1.write(0);

    }
    float data[8];
    collectData(data);
    SendData sender;
    sender.send_http(data);
    delay(1000);
}