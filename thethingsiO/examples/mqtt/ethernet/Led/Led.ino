#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

int const led = 2;

#define TOKEN "your token here"

uint8_t server[] = { 77, 73, 82, 243 };
void callback(char *, byte *, unsigned int);
EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

void setup() {
    Serial.begin(9600);
    pinMode(led, OUTPUT);

    startEthernet();

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


void startEthernet() {
    Serial.println("Connecting Arduino to network...");

    // Local Network Settings
    byte mac[] = { 0xD4, 0x28, 0xB2, 0xFF, 0xA0, 0xA1 }; // Must be unique on local network

    // Connect to network amd obtain an IP address using DHCP
    while (Ethernet.begin(mac) == 0)
        Serial.println("DHCP Failed, retrying");
    Serial.println("Arduino connected to network using DHCP");
}
