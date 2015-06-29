// Downnload ESP8266 Arduino IDE at https://github.com/esp8266/arduino
// Explained at http://makezine.com/2015/04/01/installing-building-arduino-sketch-5-microcontroller/
// Based on WiFiClient Example

#include <ESP8266WiFi.h>
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11

const char* ssid     = "here-goes-your-ssid";
const char* password = "here-goes-your-ssid-password";

const char* host = "api.thethings.io";
String APIkey = "here-goes-your-thething-api-key";

DHT dht(DHTPIN, DHTTYPE, 15);

void setup() {
  Serial.begin(9600);
  delay(10);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  dht.begin();
}

int value = 0;

void loop() {
  delay(10000);
  ++value;
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }


  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  Serial.println("Requesting URL: ");
  Serial.println(host);

  String str = "{ \"values\": [ { \"key\": \"Temp\", \"value\": \"";
  str += String(t);;
  str += "\" } ] }";
  
  Serial.println(str);
  
  client.print("POST /v2/things/");
  client.print(APIkey);
  client.print(" HTTP/1.1\n");
  client.print("Host: api.thethings.io\n");
  client.print("Connection: close\n");
  client.print("Content-Type: application/json\n");
  client.print("Content-Length: ");
  client.print(str.length());
  client.print("\n\n");
  client.print(str);
 
  delay(10);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
}

