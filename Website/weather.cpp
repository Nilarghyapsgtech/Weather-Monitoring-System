#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>



const char* server = "api.thingspeak.com";
const char* apiKey = "8GX33VU8QE8WXEH9";

WiFiClient client;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  ThingSpeak.begin(client); // Initialize ThingSpeak
}

void loop() {
  float temperature = 25.0; // Example temperature value
  float humidity = 50.0; // Example humidity value
  float rain = 10.0; // Example rain value

  // Write data to ThingSpeak
  ThingSpeak.setField(1, temperature); // Field 1: Temperature
  ThingSpeak.setField(2, humidity);    // Field 2: Humidity
  ThingSpeak.setField(3, rain);        // Field 3: Rain

  int httpCode = ThingSpeak.writeFields(THINGSPEAK_CHANNEL_ID, apiKey);

  if (httpCode == 200) {
    Serial.println("Data sent to ThingSpeak successfully");
  } else {
    Serial.println("Error sending data to ThingSpeak");
  }

  delay(60000); // Wait for 1 minute before sending the next data
}
