// Include WiFi library
#include <WiFi.h>

// Network Credentials
const char* ssid = "ssid";
const char* password = "password";

// Connection-check time interval (3 seconds)
unsigned long start = 0;
unsigned long interval = 3000;

// WiFi setup
void WiFisetup(){
// Set WiFi mode to station
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ....");
  while(WiFi.status() != WL_CONNECTED){
    Serial.println("!");
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup() {
 Serial.begin(115200);
 delay(1000);
 WiFisetup();
 Serial.print("RSSI: ");
 Serial.println(WiFi.RSSI());
}

void loop() {
// Check the current time
unsigned long current = millis();
// If WiFi is not connected, reconnect
if((WiFi.status() != WL_CONNECTED) && (current - start >= interval)){
  Serial.print(millis());
  Serial.println("Reconnecting to WiFi....");
  WiFi.reconnect();
  // Wait for 5 second for reconnect
   unsigned long attemptStart = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - attemptStart < 5000) {
      Serial.print(".");
      delay(500);
    }
  if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nReconnected!");
      Serial.print("New IP: ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("\nReconnect attempt failed.");
    }
  start = current;
}
}
