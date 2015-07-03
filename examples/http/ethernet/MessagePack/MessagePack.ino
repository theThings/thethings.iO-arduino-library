#include <SPI.h>
#include <Ethernet.h>

EthernetClient client;

#define TOKEN "your token here"

byte server[] = { 77, 73, 82, 243 };

// Simple function to serialize a key/value pair
// memory for the returned array is allocated here
// the length of the returned array is returned in the third argument
byte *msgpack(String key, String value, int &length) {
    length = 0;
    byte *data = (byte*)malloc(sizeof(byte)*22 + key.length() + value.length());
    data[length++] = 0x81;
    data[length++] = 0xa6;
    data[length++] = 'v';
    data[length++] = 'a';
    data[length++] = 'l';
    data[length++] = 'u';
    data[length++] = 'e';
    data[length++] = 's';
    data[length++] = 0x91;
    data[length++] = 0x82;
    data[length++] = 0xa3;
    data[length++] = 'k';
    data[length++] = 'e';
    data[length++] = 'y';
    data[length++] = 0xa0 + key.length();
    for (int i = 0; i < key.length(); i++) {
        data[length++] = key[i];
    }
    data[length++] = 0xa5;
    data[length++] = 'v';
    data[length++] = 'a';
    data[length++] = 'l';
    data[length++] = 'u';
    data[length++] = 'e';
    data[length++] = 0xa0 + value.length();
    for (int i = 0; i < value.length(); i++) {
        data[length++] = value[i];
    }
    return data;
}

void setup() {
    Serial.begin(9600);

    startEthernet();
}

void loop() {
    // Check and ensure that client is connected
    if (!client.connected()) {
        client.stop();
        while (!client.connect(server, 80)) {
            Serial.println("Could not connect, retrying...");
            delay(2000);
        }
        Serial.println("Connected");
    }

    // Retrieve responses
    while (client.available()) {
        Serial.print((char)client.read());
    }

    // Post the state of A0 input
    int value = analogRead(A0);
    int length;
    byte *info = msgpack("A0", String(value), length);
    POST("/v2/things/" + String(TOKEN), info, length);
    free(info);

    delay(5000);
}

void POST(String url, byte *data, unsigned int length) {
    client.print("POST " + url + " HTTP/1.1\n");
    client.print("Host: api.thethings.io\n");
    client.print("Content-Type: application/msgpack\n");
    client.print("Content-Length: ");
    client.print(length);
    client.print("\n\n");
    // use write instead of print because data must be sent in binary
    client.write(data, length);
    client.print("\n");
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
