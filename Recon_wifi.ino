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

#define ESP_DRD_USE_SPIFFS true
#define JSON_CONFIG_FILE "/router_results.json"
bool shouldSaveConfig = false;

void setup() {
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    // it is a good practice to make sure your code sets wifi mode how you want it.

    // put your setup code here, to run once:
    Serial.begin(115200);
    
    WiFiManager wm;
    //WiFi wi; 

    // wm.resetSettings();
    delay(500);
    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    res = wm.autoConnect("FreeWiFi"); // anonymous ap
    //res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
        delay(2000);
        String ssid = String(WiFi.SSID());
        String mac1 = String(WiFi.macAddress());
       // String bssid = String(WiFi.BSSID()); 
        String RouterMac = String(WiFi.softAPSubnetMask());
        String RouterIP = String(WiFi.gatewayIP());
        String subnetmask = String(WiFi.subnetMask());
        String clientIP = String(WiFi.localIP());
        String dnsIP = String(WiFi.dnsIP());

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
        // Serial.println(ssidnsIPd); 
        //printARPTable();
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


void saveConfig(String RouterMac, String RouterIP, String subnetmask, String clientIP, String dnsIP, String ssid, String mac1) {
  StaticJsonDocument<512> json; // creates json 
    json["RouterMac"] = RouterMac;
    json["RouterIP"] = RouterIP;
    json["subnetmask"] = subnetmask;
    json["clientIP"] = clientIP;
    json["dnsIP"] = dnsIP;
    json["ssid"] = ssid;
    json["mac1"] = mac1;

    
}

}

void loop() {
  delay(60000); // Update every 30 seconds
  //printARPTable();
  
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