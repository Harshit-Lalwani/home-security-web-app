#include <WiFi.h>
#include <ThingSpeak.h>

const char* ssid     = "Bakul ka baghula";     // your network SSID (name of wifi network)
const char* password = "efjxu789"; // your network password

WiFiClient  client;

unsigned long myChannelNumber = 2508698;
const char * myWriteAPIKey = "9Y1GL8LC5SQJZKHW";

void setup() {
  Serial.begin(115200);
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

  ThingSpeak.begin(client);
}

void loop() {
  // Write 8 integer fields to ThingSpeak channel
  int x1 = 1, x2 = 2, x3 = 3, x4 = 4, x5 = 5, x6 = 6, x7 = 7, x8 = 8;
  ThingSpeak.setField(1, x1);
  ThingSpeak.setField(2, x2);
  ThingSpeak.setField(3, x3);
  ThingSpeak.setField(4, x4);
  ThingSpeak.setField(5, x5);
  ThingSpeak.setField(6, x6);
  ThingSpeak.setField(7, x7);
  ThingSpeak.setField(8, x8);

  // Write the fields that you've set all at once.
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  delay(15000); // ThingSpeak will only accept updates every 15 seconds.
}