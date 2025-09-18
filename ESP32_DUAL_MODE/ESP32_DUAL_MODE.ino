// NELLY W. NDUNG'U
// ESP32 WIFI modes using Access point and Station modes simultaneously. 

// Include libraries
#include <WiFi.h>

// Network configurations
// Station mode (Router) Credentials
const char* STA_ssid = "NNW";
const char* STA_password = "nnw@88866";

// Access point (ESP) credentials
const char* AP_ssid = "ESP32_AP";
const char* AP_password = "index@2025";

// Station Mode setup
void STAsetup(){
  // Connect to WiFi
  WiFi.begin(STA_ssid, STA_password);
  Serial.print("Connecting to Wifi ....");
  while(WiFi.status() != WL_CONNECTED){
    Serial.println("!Not Connected");
    delay(1000);
  }
  // When connected, print IP address
  Serial.println(WiFi.localIP());
}

// Access point setup
void APsetup(){
  // Start Access Point
  Serial.print("Setting AP....");
  WiFi.softAP(AP_ssid, AP_password);
  Serial.println("Access Point Initiated!");
  // Print the ssid and IP address 
  Serial.println(AP_ssid);
  Serial.print(": ");
  Serial.print(WiFi.softAPIP());
}


void setup() {
  // Start Serial Monitor
  Serial.begin(115200);

  // Set WiFi mode to dual mode
  WiFi.mode(WIFI_AP_STA);
  // Connect to WiFi router/hotspot
  STAsetup();
  // Start Access Point
  APsetup();

}

void loop() {
  // put your main code here, to run repeatedly:

}
