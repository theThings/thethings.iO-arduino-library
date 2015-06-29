#include <SPI.h>
#include <Ethernet.h>
#include <thethingsiO.h>

#define TOKEN "your token here"

thethingsiOEthernet thing(TOKEN);

void setup() {
    // Start Serial for debugging on the Serial Monitor
    Serial.begin(9600);

    // Start Ethernet on Arduino
    startEthernet();
}

void loop() {
    Ethernet.maintain();

    delay(1000);
    thing.addValue("A0", "ON");
    thing.send();
    delay(1000);
    thing.addValue("A0", "OFF");
    thing.send();
}

void startEthernet() {
    Serial.println("Connecting Arduino to network...");

    // Local Network Settings
    byte mac[] = { 0xD4, 0x28, 0xB2, 0xFF, 0xA0, 0xA1 }; // Must be unique on local network

    // Connect to network amd obtain an IP address using DHCP
    while (Ethernet.begin(mac) == 0)
        Serial.println("DHCP Failed, retrying");
    Serial.println("Arduino connected to network using DHCP");
}
