#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <WiFi.h>
#include <esp_wifi.h>
#include <lwip/err.h>
#include <lwip/netif.h>
#include <lwip/ip4_addr.h>
#include <lwip/ip4.h>
#include <lwip/etharp.h>
#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <ESPmDNS.h> 
//#include <ESPAsyncWebServer.h>
#include <WebServer.h>

#define ESP_DRD_USE_SPIFFS true 
#define JSON_CONFIG_FILE "/router_results.json" // JSON FILE FOR SERIALIZATIONS/DESERIAAIN 
bool shouldSaveConfig = false; // FLAG FOR SAVING DATA 
WiFiManager wm;
WebServer server(80);

char cRouterMac[50];
char cRouterIP[50];
char csubnetmask[50];
char cclientIP[50];
char cdnsIP[50];
char cssid[50];
char cmac1[50];
String webpage = "<!DOCTYPE html><html><head><title>Page Title</title></head><body style='background-color: #EEEEEE;'><span style='color: #003366;'><h1>Lets generate a random number</h1><p>The random number is: </p></span></body></html>";
        // wm.resetSettings();
        // res = wm.autoConnect(); // auto generated AP name from chipid
        //res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

void setup() {
  
    bool forceConfig = false;      // Change to true when testing to force configuration every time we run
    bool spiffsSetup = loadConfigFile();
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    delay(500);
    Serial.begin(115200);
    delay(500);
 
  if (!spiffsSetup) {
    Serial.println(F("Forcing config mode as there is no saved config"));
    forceConfig = true;
  }


    bool res;
    res = wm.autoConnect("FreeWiFi"); // anonymous ap
    if (forceConfig){

    if(!res) {
        delay(3000);
        Serial.println("[-] Failed to connect");
        ESP.restart();
        delay(3000);

    }   
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("[+] connected :)");
        delay(2000);
        String ssid = String(WiFi.SSID());
        String mac1 = String(WiFi.macAddress());
       // String bssid = String(WiFi.BSSID()); 
        String RouterMac = String(WiFi.softAPSubnetMask());
        String RouterIP = String(WiFi.gatewayIP());
        String subnetmask = String(WiFi.subnetMask());
        String clientIP = String(WiFi.localIP());
        String dnsIP = String(WiFi.dnsIP());

    // RouterMac = WiFi.softAPmacAddress(); // Already returns a String
    // RouterIP = WiFi.gatewayIP().toString(); // Convert IPAddress to String
    // subnetmask = WiFi.subnetMask().toString(); // Convert IPAddress to String
    // clientIP = WiFi.localIP().toString(); // Convert IPAddress to String
    // dnsIP = WiFi.dnsIP().toString(); // Convert IPAddress to String
    // ssid = WiFi.SSID(); // Already returns a String
    // mac1 = WiFi.macAddress(); // Already returns a String


        delay(2000);

        processWiFiInfo(
          String(WiFi.softAPmacAddress()),
          String(WiFi.gatewayIP().toString()),
          String(WiFi.subnetMask().toString()),
          String(WiFi.localIP().toString()),
          String(WiFi.dnsIP().toString()),
          String(WiFi.SSID()),
          String(WiFi.macAddress())
      );


        delay(2000);
        server.on("/", []() {
          server.send(200, "text\html", webpage);
        });
        server.begin(); 


        delay(2000);
      saveConfig(
          String(WiFi.softAPmacAddress()),
          String(WiFi.gatewayIP().toString()),
          String(WiFi.subnetMask().toString()),
          String(WiFi.localIP().toString()),
          String(WiFi.dnsIP().toString()),
          String(WiFi.SSID()),
          String(WiFi.macAddress())
      );
      
        // Serial.println(ssidnsIPd); 
        //printARPTable();
    }
  }
}
    

void processWiFiInfo(String RouterMac, String RouterIP, String subnetmask, String clientIP, String dnsIP, String ssid, String mac1) {
    Serial.println("WiFi Information:");
    Serial.println("-------------------");
    Serial.println("Router MAC Address: " + RouterMac);
    Serial.println("Router IP Address: " + RouterIP);
    Serial.println("Subnet Mask: " + subnetmask);
    Serial.println("Client IP Address: " + clientIP);
    Serial.println("DNS IP Address: " + dnsIP);
    Serial.println("SSID: " + ssid);
    Serial.println("MAC Address: " + mac1);
    Serial.println("-------------------");
}


// void notFound(AsyncWebServerRequest *request){
//   request->send(404, "text/plain", "not found");
// }


// ---- SERIALIZES JSON (SEND FILE) -------- ///
void saveConfig(String RouterMac, String RouterIP, String subnetmask, String clientIP, String dnsIP, String ssid, String mac1) {
  
    StaticJsonDocument<512> json; // creates json 
    json["RouterMac"] = RouterMac;
    json["RouterIP"] = RouterIP;
    json["subnetmask"] = subnetmask;
    json["clientIP"] = clientIP;
    json["dnsIP"] = dnsIP;
    json["ssid"] = ssid;
    json["mac1"] = mac1;

    File configFile = SPIFFS.open(JSON_CONFIG_FILE, "w");
  
    if (!configFile)    // Error, file did not open
    {
      Serial.println("[-] failed to open config file for writing");
    }
  
    // Serialize JSON data to write to file
    serializeJsonPretty(json, Serial);
    if (serializeJson(json, configFile) == 0)
    {
      // Error writing file
      Serial.println(F("Failed to write to file"));
    }

    // Close file
    configFile.close();
}


// ------ LOADS JSON AND DESERAIALIZES (READ FILE) ------ //
bool loadConfigFile()
// Load existing configuration file
{
  // Uncomment if we need to format filesystem
  // SPIFFS.format();
 
  // Read configuration from FS json
  Serial.println("Mounting File System...");
 
  // May need to make it begin(true) first time you are using SPIFFS
  if (SPIFFS.begin(false) || SPIFFS.begin(true))
  {
    Serial.println("mounted file system");
    if (SPIFFS.exists(JSON_CONFIG_FILE))
    {
      // The file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open(JSON_CONFIG_FILE, "r");
      if (configFile)
      {
        Serial.println("Opened configuration file");
        StaticJsonDocument<512> json;
        DeserializationError error = deserializeJson(json, configFile);
        serializeJsonPretty(json, Serial);
        if (!error)
        {
          Serial.println("Parsing JSON");

    // strncpy(RouterMac, json["RouterMac"].as<String>().c_str(), siz(RouterMac) - 1);
    // strncpy(RouterIP, json["RouterIP"].as<String>().c_str(), sizeof(RouterIP) - 1);
    // strncpy(subnetmask, json["subnetmask"].as<String>().c_str(), sizeof(subnetmask) - 1);
    // strncpy(clientIP, json["clientIP"].as<String>().c_str(), sizeof(clientIP) - 1);
    // strncpy(dnsIP, json["dnsIP"].as<String>().c_str(), sizeof(dnsIP) - 1);
    // strncpy(ssid, json["ssid"].as<String>().c_str(), sizeof(ssid) - 1);
    // strncpy(mac1, json["mac1"].as<String>().c_str(), sizeof(mac1) - 1);


          return true;
        }
        else
        {
          // Error loading JSON data
          Serial.println("Failed to load json config");
        }
      }
    }
  }
  else
  {
    // Error mounting file system
    Serial.println("Failed to mount FS");
  }
 
  return false;
}
 

void saveConfigCallback()
// Callback notifying us of the need to save configuration
{
  Serial.println("[!] Should save config");
  shouldSaveConfig = true;
}
 
void configModeCallback(WiFiManager *myWiFiManager)
// Called when config mode launched
{
  Serial.println("Entered Configuration Mode");
 
  Serial.print("[+] Config SSID: ");
  Serial.println(myWiFiManager->getConfigPortalSSID());
 
  Serial.print("[+] Config IP Address: ");
  Serial.println(WiFi.softAPIP());
}
void loop() {
  delay(60000); // Update every 30 seconds
  //printARPTable();
          processWiFiInfo(
          String(WiFi.softAPmacAddress()),
          String(WiFi.gatewayIP().toString()),
          String(WiFi.subnetMask().toString()),
          String(WiFi.localIP().toString()),
          String(WiFi.dnsIP().toString()),
          String(WiFi.SSID()),
          String(WiFi.macAddress())
      );

      server.handleClient();
  
}

// void printARPTable() {
//   Serial.println("\nARP Table:");
//   Serial.println("IP Address\t\tMAC Address");

//   struct netif* netif = netif_list; // Get the network interface list
//   if (netif) {
//     for (int i = 0; i < ARP_TABLE_SIZE; i++) {
//       const struct eth_addr* mac = &arp_table[i].ethaddr;
//       const ip4_addr_t* ip = &arp_table[i].ipaddr;

//       // Check if the ARP entry is valid
//       if (!ip4_addr_isany(ip) && mac->addr[0] != 0) {
//         Serial.print(IPAddress(ip->addr));
//         Serial.print("\t\t");
//         Serial.println(macToStr(mac->addr));
//       }
//     }
//   } else {
//     Serial.println("No network interface found.");
//   }
// }

// String macToStr(const uint8_t* mac) {
//   char macStr[18];
//   snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
//            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
//   return String(macStr);
// }
