// Downnload ESP8266 Arduino IDE at https://github.com/esp8266/arduino
// Explained at http://makezine.com/2015/04/01/installing-building-arduino-sketch-5-microcontroller/
// Based on WiFiClient Example

#include <ESP8266WiFi.h>

const char* ssid     = "your-ssid-here";
const char* password = "your-wifi-pass-here";

const char* host = "api.thethings.io";
String APIkey = "your-TheThings.io-API-key here";

void setup() {
  Serial.begin(115200);
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
}

int value = 0;

void loop() {
  delay(20000);
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  Serial.print("Requesting URL: ");
  //Serial.println(url);

  String str = "{ \"values\": [ { \"key\": \"A3\", \"value\": \"";
  str += "10";    // Here goes the upload/measured value
  str += "\" } ] }";
  
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

