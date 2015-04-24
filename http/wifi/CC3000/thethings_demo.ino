// Based on: https://github.com/openhomeautomation/wireless-gardening-arduino
// Created by Hans Crijns, CloudSensor.us

#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"

// Define CC3000 chip pins
#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10

// Create CC3000 instances
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIV2);

// WLAN & theThings.io parameters
#define WLAN_SSID       "fill-you-yourself"
#define WLAN_PASS       "fill-you-yourself"
#define WLAN_SECURITY   WLAN_SEC_WPA2 // Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WEBSITE         "api.thethings.io"

String APIkey = "fill-you-yourself";
uint32_t ip;

void setup(void){
  Serial.begin(115200);
  Serial.println("theThings.io demo");
  Serial.println(F("=> Initializing..."));
  if (!cc3000.begin()) {
    Serial.println(F("Couldn't begin()! Check your wiring?"));
    while(1);
  }
}

void loop(void) {
  // Connect to WiFi network
  Serial.println(F("=> Connect to WiFi"));
  cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY);

  Serial.println(F("=> Request DHCP"));
  while (!cc3000.checkDHCP()) {
    delay(100);
  }

  // Get the website IP & print it
  ip = 0;
  Serial.print(WEBSITE); Serial.print(F(" -> "));
  while (ip == 0) {
    if (! cc3000.getHostByName(WEBSITE, &ip)) {
      Serial.println(F("Couldn't resolve!"));
    }
    delay(500);
  }
  cc3000.printIPdotsRev(ip);

  // Get data ready (analog value A1)
  String analogValue = String(analogRead(A1), DEC);

  String data = "{ \"values\": [ { \"key\": \"A1\", \"value\": \"";
  data += analogValue;
  data += "\" } ] }";
  Serial.println(data);

  // Send request
  Adafruit_CC3000_Client client = cc3000.connectTCP(ip, 80);
  if (client.connected()) {
    Serial.println("Connected!");
    client.print("POST /v2/things/");
    client.print(APIkey);
    client.print(" HTTP/1.1\n");
    client.print("Host: api.thethings.io\n");
    client.print("Connection: close\n");
    client.print("Content-Type: application/json\n");
    client.print("Content-Length: ");
    client.print(data.length());
    client.print("\n\n");
    client.print(data);

  } else {
    Serial.println(F("Connection failed"));
    return;
  }

  Serial.println(F("-------------------------------------"));
  while (client.connected()) {
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
  }
  client.close();
  Serial.println(F("-------------------------------------"));

  Serial.println(F("\n\nDisconnecting"));
  cc3000.disconnect();

  // Wait 60 seconds until next update
  delay(60000);
}
