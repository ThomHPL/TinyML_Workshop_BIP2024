/*
 *  This sketch demonstrates how to scan WiFi networks.
 *  The API is based on the Arduino WiFi Shield library, but has significant changes as newer WiFi functions are supported.
 *  E.g. the return value of `encryptionType()` different because more modern encryption is supported.
 */
#include "WiFi.h"

typedef struct {
  String ssid = "";
  int32_t channel = 0;
  int32_t RSSI;
} Network;

#define MAX_NODES 4
Network esp_networks[MAX_NODES];
uint8_t detected_beacons = 0;

void setup()
{
    Serial.begin(115200);

    // Set WiFi to station mode and disconnect from an AP if it was previously connected.
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    int n = WiFi.scanNetworks(false, false, false, 300);
    uint8_t j = 0;
    for (int i = 0; i < n && i < MAX_NODES; i++) {
      char tmp[] = "   ";
      memcpy(tmp, WiFi.SSID(i).c_str(), 3);
      if(!strcmp(tmp, "ESP")){
        esp_networks[j].ssid = WiFi.SSID(i);
        Serial.printf("%s\t", esp_networks[j].ssid);
        esp_networks[j].channel = WiFi.channel(i);
        esp_networks[j].RSSI = WiFi.RSSI(i);
        j ++;
      }
    }
    Serial.printf("\n");
    WiFi.scanDelete();
    detected_beacons = j;
}

void loop()
{
  for (int k = 0; k < detected_beacons; k++) {
    int n = WiFi.scanNetworks(false, false, false, 100, esp_networks[k].channel);
    for (int i = 0; i < n && i < MAX_NODES; i++) {
      if(WiFi.SSID(i) == esp_networks[k].ssid) {
        esp_networks[k].RSSI = WiFi.RSSI(i);
        Serial.printf("%d\t", esp_networks[k].RSSI);
      }
    }
    WiFi.scanDelete();
  }
  Serial.printf("\n");
}
