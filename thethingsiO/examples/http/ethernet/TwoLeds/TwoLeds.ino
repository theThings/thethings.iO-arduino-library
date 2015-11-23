#include <SPI.h>
#include <Ethernet.h>
#include <thethingsiO.h>

#define FIRST_TOKEN "your token here"
#define SECOND_TOKEN "your token here"

const int  first_led = 2;
const int  second_led = 5;

thethingsiOEthernet first_thing(FIRST_TOKEN);
thethingsiOEthernet second_thing(SECOND_TOKEN);

void setup() {
    Serial.begin(9600);

    pinMode(first_led, OUTPUT);
    pinMode(second_led, OUTPUT);

    digitalWrite(first_led, HIGH);
    digitalWrite(second_led, HIGH);

    startEthernet();

    digitalWrite(first_led, LOW);
    digitalWrite(second_led, LOW);

    first_thing.subscribe();
    second_thing.subscribe();
}

void loop() {
    checkThing(first_thing, first_led);
    checkThing(second_thing, second_led);
    checkSubscription(first_thing);
    checkSubscription(second_thing);
}

void checkThing(thethingsiOEthernet &thing, const int led) {
    // Parse new data on the Thing to turn the led on/off
    if (thing.available()) {
        String text = thing.read();
        text.replace(" ", "");
        text.toUpperCase();
        if (text.indexOf("\"KEY\":\"LED\"") >= 0) {
            if (text.indexOf("\"VALUE\":\"ON\"") >= 0) digitalWrite(led, HIGH);
            else if (text.indexOf("\"VALUE\":\"OFF\"") >= 0) digitalWrite(led, LOW);
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
