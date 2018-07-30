#include <ESP8266WiFi.h>
#include <thethingsiO.h>

#define TOKEN "your token here"
#define SSID "your ssid here"
#define PASSWORD "your ssid password here"

#define button_pin 2

thethingsiOWiFi thing(TOKEN);
bool pressed = false;

void setup() {
    // initialize serial communications
    Serial.begin(9600);

    WiFi.begin(SSID, PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    thing.subscribe();
}

void loop() {
    if (not pressed and digitalRead(button_pin) == LOW) {
        Serial.println("BUTTON PRESSED");
        delay(100);
        pressed = true;
        thing.addValue("button", 1);
        thing.send();
    }
    else if (pressed and digitalRead(button_pin) != LOW) {
        Serial.println("BUTTON RELEASED");
        delay(100);
        pressed = false;
    }
}
