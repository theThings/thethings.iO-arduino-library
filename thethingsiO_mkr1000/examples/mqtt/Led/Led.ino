#include <WiFi101.h>
#include <SPI.h>
#include <thethingsiO_mkr1000.h>
#include <thethingsiOWiFi_mkr1000.h>
#include <thethingsiOClient_mkr1000.h>
#include <PubSubClient.h>

int const led = 6;

#define TOKEN "zR_I1SfhRZnAgIIQ0WkhjVxzlEVN9jei4I_2MJitSXs"
#define WIFI_AP "thethings.iO - HQ"
#define WIFI_PWD "ilovethethingsioTTIO007"
char* server = "mqtt.thethings.io";
void callback(char *, byte *, unsigned int);
WiFiClient wifiClient;
PubSubClient client(server, 1883, callback, wifiClient);
int status = -1;

void setup() {
    Serial.begin(115200);
    pinMode(led, OUTPUT);

    startWifi();

    while (!client.connect("arduinoclient")) {
        Serial.println("Could not subscribe, retrying...");
    }
    Serial.println("Client connected to mqtt broker!");

    String topic = "v2/things/" + String(TOKEN);
    client.subscribe((char*)topic.c_str());

}

void loop() {
    client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
    // handle message arrived
    String text = "";
    for (int i = 0; i < length; i++)
        text.concat((char)payload[i]);
    Serial.println(text);
    text.replace(" ", "");
    text.toUpperCase();
    if (text.indexOf("\"KEY\":\"LED\"") >= 0) {
        if (text.indexOf("\"VALUE\":\"ON\"") >= 0) digitalWrite(led, HIGH);
        else if (text.indexOf("\"VALUE\":\"OFF\"") >= 0) digitalWrite(led, LOW);
    }
}


void startWifi(){
  Serial.println("Connecting MKR1000 One to network...");
   // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED ) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(WIFI_AP);
    WiFi.begin(WIFI_AP, WIFI_PWD);
    // wait 10 seconds for connection:
    delay(5000);
    status = WiFi.status();
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();
}

void printWifiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  Serial.print(bssid[5],HEX);
  Serial.print(":");
  Serial.print(bssid[4],HEX);
  Serial.print(":");
  Serial.print(bssid[3],HEX);
  Serial.print(":");
  Serial.print(bssid[2],HEX);
  Serial.print(":");
  Serial.print(bssid[1],HEX);
  Serial.print(":");
  Serial.println(bssid[0],HEX);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);
}
