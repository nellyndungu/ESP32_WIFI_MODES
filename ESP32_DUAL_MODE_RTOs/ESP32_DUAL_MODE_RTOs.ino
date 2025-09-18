// NELLY W. NDUNG'U
// Use of RTOs 
// ESP32 WIFI modes using Access point and Station modes simultaneously. 

// Include libraries
#include <WiFi.h>
#include <esp_wifi.h>

// Network configurations
// Station mode (Router) Credentials
const char* STA_ssid = "ssid";
const char* STA_password = "password";

// Access point (ESP) credentials
const char* AP_ssid = "ssid";
const char* AP_password = "password";

// Station mode connection-check interval (3 seconds)
unsigned long start = 0;
unsigned long interval = 3000;

// Station Mode setup
void STAsetup(){
  // Connect to WiFi
  WiFi.begin(STA_ssid, STA_password);
  Serial.print("[STA]: Connecting to Wifi ....");
  while(WiFi.status() != WL_CONNECTED){
    Serial.println("[STA]: !Not Connected");
    delay(1000);
  }
  // When connected, print IP address
  Serial.println(WiFi.localIP());
  // Get and print Wifi strength 
  Serial.print("[STA]: RSSI: ");
  Serial.println(WiFi.RSSI());
}

// Connection check and reconnection task
void STAinterval(void *parameters){
  while(1){
  // Check the current time
  unsigned long current = millis();
  if((WiFi.status() != WL_CONNECTED) && (current - start >= interval)){
    Serial.print("[STA]: Reconnecting to WiFi.....");
    WiFi.reconnect();
    // Wait for 5 second for reconnecting
    unsigned long attemptStart = millis();
    while(WiFi.status() != WL_CONNECTED && millis() - attemptStart < 5000){
      Serial.print("[STA]: ! Not Connected");
      vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    if(WiFi.status() == WL_CONNECTED){
      Serial.println("[STA]: Reconnected");
      Serial.print("[STA]: New IP: ");
      Serial.println(WiFi.localIP());
    }else {
      Serial.println("[STA]: !Reconnect Failed");
    }
    start = current;
  }
  // Delay to yield CPU
  vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// Access point setup
void APsetup(){
  // Start Access Point
  Serial.print("[AP]: Setting AP....");
  WiFi.softAP(AP_ssid, AP_password);
  Serial.println("[AP]: Access Point Initiated!");
  // Print the ssid and IP address 
  Serial.printf("[AP]: %s : %s\n", AP_ssid, WiFi.softAPIP().toString().c_str());

}

// List connected devices task
void APList(void *parameters){
  while(1){
  // Get number of stations connected
int stationsNum = WiFi.softAPgetStationNum();

//Print the number of connected stations
Serial.print("[AP]: Connected devices: ");
Serial.println(stationsNum);

if(stationsNum > 0){
  wifi_sta_list_t stationList;
  esp_wifi_ap_get_sta_list (&stationList);
  Serial.printf("[AP]: Connected devices: %d\n", stationList.num);

for (int i = 0; i < stationList.num; i++) {
    wifi_sta_info_t station = stationList.sta[i];

     Serial.printf("[AP]: Device %d - MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
                  i + 1,
                  station.mac[0], station.mac[1], station.mac[2],
                  station.mac[3], station.mac[4], station.mac[5]);
  }
}
// Check every 1s
vTaskDelay(1000 / portTICK_PERIOD_MS);
}
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
  // Create tasks
  xTaskCreatePinnedToCore(STAinterval, "STA Connection", 4096, NULL, 1, NULL, 0); // First Core
  xTaskCreatePinnedToCore(APList, "AP Device List", 4096, NULL, 1, NULL, 1); // Second Core
}

void loop() {
  
}
