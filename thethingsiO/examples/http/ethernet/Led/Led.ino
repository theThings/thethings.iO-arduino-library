#include <SPI.h>
#include <Ethernet.h>
#include <thethingsiO.h>

#define TOKEN "your token here"

const int  ledPin = 2;

thethingsiOEthernet thing(TOKEN);

void setup() {
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);
    startEthernet();
    digitalWrite(ledPin, LOW);
    thing.subscribe();
}

void loop() {
    checkThing(thing, ledPin);
    checkSubscription(thing);
}

void checkThing(thethingsiOEthernet &thing, const int led) {
    // Parse new data on the Thing to turn the led on/off
    if (thing.available()) {
        String text = thing.read();
        text.replace(" ", "");
        text.toUpperCase();
        if (text.indexOf("\"KEY\":\"LED\"") >= 0) {
            if (text.indexOf("\"VALUE\":\"ON\"") >= 0) {
                Serial.println("Led: ON");
                digitalWrite(led, HIGH);
            }
            else if (text.indexOf("\"VALUE\":\"OFF\"") >= 0) {
                Serial.println("Led: OFF");
                digitalWrite(led, LOW);
            }
        }
    }
}

void checkSubscription(thethingsiOEthernet &thing) {
    if (!thing.subscribed()) {
        Serial.println("Resubscribe!");
        thing.subscribe();
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
