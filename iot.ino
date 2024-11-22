#include <ESP8266WiFi.h>
const char* ssid = "Your_Wifi name";
const char* password = "Your_PASSWORD";

// ThingSpeak settings
// Replace Your_Wifi name and Your_PASSWORD with your WiFi credentials.
// Replace Your_API_KEY with the Write API Key from ThingSpeak.

const char* host = "api.thingspeak.com";
const String apiKey = "Your_API_KEY";

int soilPin = A0;  // Analog pin connected to soil sensor
int soilValue;

void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
}

void loop() {
  // Read soil moisture value
  soilValue = analogRead(soilPin);
  Serial.print("Soil Moisture: ");
  Serial.println(soilValue);

  // Send data to ThingSpeak
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    if (client.connect(host, 80)) {
      String postStr = "api_key=" + apiKey + "&field1=" + String(soilValue);
      client.println("POST /update HTTP/1.1");
      client.println("Host: api.thingspeak.com");
      client.println("Connection: close");
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      client.println(postStr.length());
      client.println();
      client.print(postStr);
      Serial.println("Data sent to ThingSpeak");
    }
    client.stop();
  }
  delay(20000);  // ThingSpeak allows updates every 15 seconds
}
