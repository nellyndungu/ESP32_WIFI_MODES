// Access point - the stations/devices connect to ESP32
// Good for wireless communication between one ESP32 and other devices without needing a router. 
// Include WiFi library
#include <WiFi.h>
#include "esp_wifi.h"

// Set network credentials
const char* ssid = "ssid";
const char* password = "password";

void setup(){
  Serial.begin(115200);

  Serial.print("Setting AP ...");
  //Start ESP32 in Access Point Mode
  WiFi.softAP(ssid, password);

  // Print network details
  Serial.println("Access Point Initiated!");
  Serial.println(ssid);
  Serial.print(": ");
  Serial.print(WiFi.softAPIP());
}
void loop(){
// Get number of stations connected
int stationsNum = WiFi.softAPgetStationNum();

//Print the number of connected stations
Serial.print("Connected devices: ");
Serial.println(stationsNum);

if(stationsNum > 0){
  wifi_sta_list_t stationList;
  esp_wifi_ap_get_sta_list (&stationList);
  Serial.printf("Connected devices: %d\n", stationList.num);

for (int i = 0; i < stationList.num; i++) {
    wifi_sta_info_t station = stationList.sta[i];

     Serial.printf("Device %d - MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
                  i + 1,
                  station.mac[0], station.mac[1], station.mac[2],
                  station.mac[3], station.mac[4], station.mac[5]);
  }
}
delay(500);
}