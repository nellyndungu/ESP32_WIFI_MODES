# ESP32 Wi-Fi Modes: Station, Access Point and Dual Mode (With and Without RTOS)
## Overview
The ESP32 is a Wi-Fi–enabled microcontroller that can function as both a client and a host within a network. In Station mode (STA), it connects to an existing network, while in Access Point mode (AP), it creates its own network for other devices to join. It can also operate in dual mode, handling both roles simultaneously. This repository contains firmware developed in the Arduino IDE and NodeMCU, showcasing how the ESP32 leverages these networking capabilities.

## Station Mode
In Station mode, the ESP32 connects to a network device specified in the firmware. Once the connection is established, the Serial Monitor displays the status as Connected, along with the ESP32’s IP address and the Received Signal Strength Indicator (RSSI), which reflects the connection quality. The firmware continuously checks the connection status every 3 seconds. If the ESP32 is disconnected, it automatically attempts to reconnect.
### Firmware Development
1. Network Configuration

The network credentials **(ssid and password)** and timing constants (interval for checking status and timeout for reconnection) are defined. The credentials are used to establish a Wi-Fi connection, while the timing values guide when to verify or reattempt connection.

2. Set Wi-Fi Mode and Initiate Connection

The ESP32 is configured as a station using the API **WiFi.mode(WIFI_STA)**. A connection request is then initiated with **WiFi.begin(ssid, password)**. Once connected, the device prints its assigned IP address using **WiFi.localIP()** and the signal strength (RSSI) using **WiFi.RSSI()**.

3. Check Wi-Fi Connection
The connection status is monitored periodically with **WiFi.status()**. Every 3 seconds, the firmware checks if the ESP32 is still connected. If disconnected, it attempts to reconnect using **WiFi.reconnect()**, allowing time for the reconnection process before reporting success or failure.

## Access Point Mode
In Access Point mode, the ESP32 creates its own Wi-Fi network using the credentials defined in the firmware. Other devices can then connect directly to the ESP32. Once a device connects, the firmware prints the total number of connected devices and displays details client's MAC address.
### Firmware Development
1. Network Configuration

The SSID and password for the ESP32’s access point are defined. These credentials are required by external devices to join the network.

2. Set Wi-Fi Mode and Start Access Point

The ESP32 is configured as an access point using **WiFi.mode(WIFI_AP)** and the network is started with **WiFi.softAP(ssid, password)**.

3. Monitor Connected Devices

Connected clients are tracked using **WiFi.softAPgetStationNum()** to retrieve the number of active connections. For each connected device, its MAC address is obtained using **WiFi.softAPgetStationInfo()** and printed to the Serial Monitor.

## Dual Mode (STA + AP)
In Dual Mode, the ESP32 operates as both a station and an access point simultaneously. This allows it to connect to a router or hotspot (like in Station Mode) while also letting external devices connect directly to it (like in Access Point Mode). The firmware therefore combines:
- Station Mode functionality: connecting to an existing network, printing IP, monitoring RSSI, and performing reconnection if disconnected.
- Access Point functionality: hosting a Wi-Fi network, accepting device connections, and listing connected clients with their MAC addresses.

### Dual Mode without RTOS
In this version, the firmware executes both station and access point functions sequentially inside the loop(). The connection status is checked periodically using timing logic, while client devices are listed with each iteration. This approach works but keeps all operations running in a single thread, which can affect responsiveness as the system grows more complex.

### Dual Mode with RTOS
With RTOS, the firmware leverages multitasking to handle station and access point operations concurrently on different cores.
Sequence of Functionality:
1. Dual Mode

ESP32 is configured in dual mode using **WiFi.mode(WIFI_AP_STA).**
2. Station Task (STA)

- Setup: A connection request is then initiated with **WiFi.begin(ssid, password)**., then the local IP and RSSI are displayed.
- Task (STAinterval): A FreeRTOS task continuously monitors connection status using WiFi.status(). Every 3 seconds, it attempts reconnection if disconnected, prints results, and yields CPU with vTaskDelay().
3. Access Point Task (AP)

- Setup: The access point is initialized with **WiFi.softAP()** and the SSID and IP address are displayed using **WiFi.softAPIP()**.
- Task (APList): Another FreeRTOS task retrieves the number of connected devices with **WiFi.softAPgetStationNum()**. It then uses **esp_wifi_ap_get_sta_list()** to fetch each client’s MAC address, printing them to the serial monitor at 1-second intervals.
4. Task Scheduling and Core Assignment

xTaskCreatePinnedToCore() is used to assign the STAinterval task to Core 0 and the APList task to Core 1. This separation ensures that station reconnection handling and access point client listing run independently and do not block each other.
