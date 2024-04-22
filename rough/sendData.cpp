#include <WiFi.h>
#include <HTTPClient.h>

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

void collectData(float data[8]) {
    data[0]=  0;
    data[1] = getDistance();
    data[2] = touchRead(4);
    data[3]  = digitalRead(IR_SENSOR_PIN);
    data[4]  =0;
    data[5] = digitalRead(IR_SENSOR_PIN2);
    data[6] = 0;
    data[7] = 0;
}
void setup() {
    Serial.begin(9600);
    SendData sender;
    sender.setup_wifi();
    // float data[8] = {10.5, 20.3, 30.1, 40.8, 50.2, 60.7, 70.4, 80.6}; // Sample data, replace with your actual data
    // sender.send_http(data);
}

void loop() {
    float data[8];
    collectData(data);
    SendData sender;
    sender.send_http(data);
    delay(15000);
}
