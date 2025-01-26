Here’s a README.md for your project:

WiFiManager & WebSocket-based Router Information Display

This project is an ESP32-based Wi-Fi manager and web server that captures and displays router information, such as router MAC address, IP address, subnet mask, DNS IP, and connected client IP. It includes functionality to connect to a Wi-Fi network, display router info in a webpage, and send the info over WebSockets to a connected client.

Features
	•	Wi-Fi Manager: Automatically connects to available Wi-Fi or enters configuration mode to set up Wi-Fi credentials.
	•	Web Server: Hosts a simple HTML page displaying a random number generator.
	•	WebSocket Server: Allows real-time communication with the browser or any WebSocket client.
	•	ESP32-based: Uses ESP32’s Wi-Fi capabilities to gather network details.
	•	JSON Configuration File: Saves the router’s details in a JSON file, stored in SPIFFS (Serial Peripheral Interface Flash File System), for persistent storage.

Dependencies
	•	WiFiManager: WiFiManager Library to simplify the Wi-Fi connection management.
	•	ArduinoJson: For handling JSON file reading/writing.
	•	ESP32 Libraries: For Wi-Fi, mDNS, and WebSocket functionality.
	•	SPIFFS: For file system support on ESP32.

Installation
	1.	Install Libraries:
	•	WiFiManager: #include <WiFiManager.h>
	•	WiFi: #include <WiFi.h>
	•	ESP32 Libraries: #include <esp_wifi.h>
	•	ArduinoJson: #include <ArduinoJson.h>
	•	WebServer: #include <WebServer.h>
	•	WebSocketsServer: #include <WebSocketsServer.h>
	•	SPIFFS: #include <SPIFFS.h>
	2.	Upload the Sketch:
	•	Open the .ino file in Arduino IDE or PlatformIO and upload it to your ESP32 board.
	•	Ensure the ESP32 board and port are correctly selected in the Arduino IDE.
	3.	Configuration:
	•	On the first boot, the device will create an access point named “FreeWiFi”.
	•	Connect to this AP and access the configuration portal to enter your Wi-Fi credentials.

Usage
	1.	Wi-Fi Connection:
	•	If the device has a saved configuration, it will automatically connect to the Wi-Fi network.
	•	If no configuration is found, the device will enter configuration mode where you can set the Wi-Fi credentials.
	2.	Web Server:
	•	Once connected to Wi-Fi, open a browser and navigate to the ESP32’s IP address.
	•	You will be presented with a simple webpage showing a random number.
	3.	WebSocket:
	•	The device also runs a WebSocket server to communicate with connected clients.
	•	It sends the router’s details to the WebSocket client.

Code Overview
	•	WiFiManager: Used for connecting the ESP32 to a Wi-Fi network.
	•	WebServer: Hosts a simple HTML page displaying a random number.
	•	WebSocketsServer: Manages WebSocket connections and handles real-time communication with the browser.
	•	JSON Configuration: Saves the router’s details in a JSON file (router_results.json) stored in SPIFFS for persistence.

Functions

setup()
	•	Initializes the Wi-Fi connection.
	•	Creates a configuration mode if no previous configuration is found.
	•	Starts the web server and WebSocket server.

onWebSocketEvent()
	•	Handles WebSocket events like new connections and incoming messages.
	•	Sends messages back to WebSocket clients.

processWiFiInfo()
	•	Prints the router’s Wi-Fi information to the serial monitor.

saveConfig()
	•	Saves Wi-Fi details (MAC address, IP, subnet mask, DNS) to a JSON file on SPIFFS.

loadConfigFile()
	•	Loads the saved configuration from SPIFFS.

Configuration File Format

The configuration file (router_results.json) contains the following fields:

{
  "RouterMac": "xx:xx:xx:xx:xx:xx",
  "RouterIP": "192.168.1.1",
  "subnetmask": "255.255.255.0",
  "clientIP": "192.168.1.100",
  "dnsIP": "8.8.8.8",
  "ssid": "YourWiFiSSID",
  "mac1": "xx:xx:xx:xx:xx:xx"
}

Troubleshooting
	•	Wi-Fi Connection Issues: If the device cannot connect to the Wi-Fi, ensure the credentials are correct, or reset the device to enter configuration mode again.
	•	Webpage Not Loading: Ensure the device is properly connected to the network and you are accessing the correct IP address of the ESP32.

License

This project is licensed under the MIT License - see the LICENSE file for details.

Let me know if you need further adjustments or additions!
