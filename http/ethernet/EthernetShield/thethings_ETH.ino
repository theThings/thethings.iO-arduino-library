// Based on: https://github.com/iobridge/ThingSpeak-Arduino-Examples/blob/master/Ethernet/Arduino_to_ThingSpeak.ino

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xD4, 0x28, 0xB2, 0xFF, 0xA0, 0xA1 }; // Must be unique on local network

char serverAddress[] = "api.thethings.io";
String APIkey = "your thing token goes here";
const int updateInterval = 16 * 1000;      // Time interval in milliseconds to update (number of seconds * 1000 = interval)

long lastConnectionTime = 0;
boolean lastConnected = false;
int failedCounter = 0;

EthernetClient client;

void setup() {
  Serial.begin(9600);
  startEthernet();
}

void loop() {
  String analogValue0 = String(analogRead(A0), DEC);
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // Disconnect
  if (!client.connected() && lastConnected) {
    Serial.println("...disconnected");
    Serial.println();
    client.stop();
  }

  // Update values; send POST
  if(!client.connected() && (millis() - lastConnectionTime > updateInterval)) {
    String str = "{ \"values\": [ { \"key\": \"A0\", \"value\": \"";
    str += analogValue0;
    str += "\" } ] }";

    updatePOST(str);
    Serial.print("POST: ");
    Serial.println(str);
  }

  // Check if Arduino Ethernet needs to be restarted
  if (failedCounter > 3 ) {
    startEthernet();
  }
  lastConnected = client.connected();
}

void updatePOST(String tsData) {
  if (client.connect(serverAddress, 80)) {
    client.print("POST /v2/things/");
    client.print(APIkey);
    client.print(" HTTP/1.1\n");
    client.print("Host: api.thethings.io\n");
    client.print("Connection: close\n");
    client.print("Content-Type: application/json\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");
    client.print(tsData);

    lastConnectionTime = millis();

    if (client.connected()) {
      Serial.println("Connecting...");
      Serial.println();
      failedCounter = 0;
    }
    else {
      failedCounter++;
      Serial.println("Connection failed ("+String(failedCounter, DEC)+")");
      Serial.println();
    }
  }
  else {
    failedCounter++;
    Serial.println("Connection Failed ("+String(failedCounter, DEC)+")");
    Serial.println();
    lastConnectionTime = millis();
  }
}

void startEthernet() {
  client.stop();
  Serial.println("Connecting Arduino to network...");
  Serial.println();
  delay(1000);

  // Connect to network amd obtain an IP address using DHCP
  if (Ethernet.begin(mac) == 0) {
    Serial.println("DHCP Failed, reset Arduino to try again");
    Serial.println();
  }
  else {
    Serial.println("Arduino connected to network using DHCP");
    Serial.println();
  }
  delay(1000);
}
