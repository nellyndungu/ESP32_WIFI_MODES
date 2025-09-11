// Include WiFi, web server and time libraries
#include <WiFi.h>
#include <WebServer.h>
#include "time.h"

// Include the webpage content file
#include <Webpage.h>

// Network Credentials
const char* ssid = "ssid";
const char* password = "password";

// NTP Configuration
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3 * 3600;   // Example: GMT+3 for Kenya
const int daylightOffset_sec = 0;

// Connection-check time interval (3 seconds)
unsigned long start = 0;
unsigned long interval = 3000;

//Create object to listen on port 80
WebServer server(80);

// Ticket data variables
String ticketName = "";
String ticketTime = "";
String ticketID = "";

//Generate ticket
String generateTicket(String name){
  String id = "";
  name.toUpperCase();
  if(name.length() >= 2){
    id += name.substring(0, 2);
  } else{
    //Create fallback
    id += "XX";
  }
   // Reseed randomness a little each call using micros()
  randomSeed(micros() ^ esp_random());
  // Pick 3 random digits
  id += String(random(100, 999));
  return id;
}

// Get current time as string
String getCurrentTime() {
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    char buffer[30];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return String(buffer);
  } else {
    return "Time unavailable";
  }
}

// Serve webpage with ticket
void handleRoot(){
  String page = MAIN_page;
  if (ticketName != "") {
    page.replace("%NAME%", ticketName);
    page.replace("%TIME%", ticketTime);
    page.replace("%ID%", ticketID);
  } else {
    page.replace("%NAME%", "No ticket yet");
    page.replace("%TIME%", "-");
    page.replace("%ID%", "---");
  }
  server.send(200, "text/html", page);
}

// Handle ticket generation from form
void handleGenerate() {
  if (server.hasArg("name")) {
    ticketName = server.arg("name");
    ticketID = generateTicket(ticketName);
    // use actual NTP time
    ticketTime = getCurrentTime();   
  }
  server.sendHeader("Location", "/");
  server.send(303);
}


// Handle download ticket as PDF
void handleDownloadPDF() {
  if (ticketName == "") {
    server.send(200, "text/plain", "No ticket available. Please generate one first.");
    return;
  }

  // Very simple PDF with the ticket details
  String pdf = "%PDF-1.1\n";
  pdf += "1 0 obj << /Type /Catalog /Pages 2 0 R >> endobj\n";
  pdf += "2 0 obj << /Type /Pages /Kids [3 0 R] /Count 1 >> endobj\n";
  pdf += "3 0 obj << /Type /Page /Parent 2 0 R /MediaBox [0 0 300 200] /Contents 4 0 R /Resources << >> >> endobj\n";

  // Ticket text
  String content = "BT /F1 12 Tf 50 150 Td (ESP32 Ticket) Tj ET\n";
  content += "BT /F1 10 Tf 50 130 Td (Name: " + ticketName + ") Tj ET\n";
  content += "BT /F1 10 Tf 50 115 Td (Time: " + ticketTime + ") Tj ET\n";
  content += "BT /F1 10 Tf 50 100 Td (ID: " + ticketID + ") Tj ET\n";

  int length = content.length();
  pdf += "4 0 obj << /Length " + String(length) + " >> stream\n";
  pdf += content;
  pdf += "endstream endobj\n";
  pdf += "xref\n0 5\n0000000000 65535 f \n";
  pdf += "trailer << /Size 5 /Root 1 0 R >>\n";
  pdf += "startxref\n";
  pdf += String(pdf.length() - 20) + "\n%%EOF\n";

  // Force browser to download as PDF
  server.sendHeader("Content-Type", "application/pdf");
  server.sendHeader("Content-Disposition", "attachment; filename=\"ticket.pdf\"");
  server.send(200, "application/pdf", pdf);
}

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

  // Init NTP
   configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    Serial.println(&timeinfo, "Time synced: %Y-%m-%d %H:%M:%S");
  } else {
    Serial.println("Failed to obtain time from NTP");
  }
}

void setup() {
 Serial.begin(115200);
 delay(1000);
 Serial.print("RSSI: ");
 Serial.println(WiFi.RSSI());

 // Initialize random generator
 randomSeed(esp_random());
 WiFisetup();
 
 // Setup routes
 server.on("/", handleRoot);
 server.on("/generate", handleGenerate); 
 server.on("/download", handleDownloadPDF);
 server.begin();

Serial.println("Enter Name for ticket:");
}

void loop() {
  server.handleClient();
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
}}
