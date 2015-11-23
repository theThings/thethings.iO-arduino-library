// Based on: https://github.com/iobridge/ThingSpeak-Arduino-Examples/blob/master/Ethernet/Arduino_to_ThingSpeak.ino

#include <SPI.h>
#include <Ethernet.h>
#include <thethingsiO.h>

#define ACTIVATION_CODE "your activation code here"

thethingsiOEthernet thing;
const int led = 3;

void setup() {
    // Start Serial for debugging on the Serial Monitor
    Serial.begin(9600);

    pinMode(led, OUTPUT);

    // Start Ethernet on Arduino
    startEthernet();

    // Activate the new Thing and print the Token
    thing.activate(ACTIVATION_CODE);
    Serial.println("token: " + thing.getToken());

    // Subscribe to new messages on the activated Thing
    Serial.println("Subscribing...");
    thing.subscribe();
}

void loop() {
    Ethernet.maintain();

    // Parse new data on the Thing to turn the led on/off
    if (thing.available()) {
        String text = thing.read();
        text.replace(" ", "");
        text.toUpperCase();
        if (text.indexOf("\"KEY\":\"A0\"") >= 0) {
            if (text.indexOf("\"VALUE\":\"ON\"") >= 0) digitalWrite(led, HIGH);
            else if (text.indexOf("\"VALUE\":\"OFF\"") >= 0) digitalWrite(led, LOW);
        }
    }
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
