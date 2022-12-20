#include "wl_definitions.h"
#include "WiFiClient.h"
#include "IPAddress.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>


String ip = "IP unset";
String password = "12345678";
String ssid_prefix = "ESP8266_";

String ssidCLI = "Kuply Slona";
String passwordCLI = "12345678";

ESP8266WiFiMulti wifiClient;
WiFiClient network_client;

String id(){
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) + String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  return macID;
}

bool startAPMode(){
  IPAddress apIP(192, 168, 4, 1);
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid_prefix + id(), password);
  Serial.println("WIFI AP is up with " + ssid_prefix + id());
  return true;
}

bool StartCLI(){
  wifiClient.addAP(ssidCLI.c_str(), passwordCLI.c_str());
  while (wifiClient.run() != WL_CONNECTED){

  }
  return true;
}
void WiFi_init(bool ap_mode){
  if (ap_mode){
    startAPMode();
  }
  else{
    StartCLI();
    ip = WiFi.localIP().toString();
  }
  Serial.println("Local IP is: " + ip);
}