#include "example.pb.h"
#include "pb.h"
#include "pb_encode.h"
#include <Ethernet.h>
#include <SPI.h>

#define TOKEN "your token here"

uint8_t buffer[50];

EthernetClient client;

long const delta_millis = 10000;
long long last_millis = 0;

void setup() {
    // Start Serial for debugging on the Serial Monitor
    Serial.begin(9600);

    // Start Ethernet on Arduino
    startEthernet();
}

void loop() {
    // Check for the EthernetClient to be connected, reconnect if necessary
    if (!client.connected()) {
        byte server[] = { 77, 73, 82, 243 };
        client.stop();
        while (!client.connect(server, 80)) {
            Serial.println("Could not connect, retrying...");
            delay(2000);
        }
        Serial.println("Connected");
    }

    // Send info if more than 10s (delta_millis) have passed since last time
    if (millis() - last_millis > delta_millis) {
        sendValue(analogRead(A0));
        last_millis = millis();
    }

    // Output any received data
    if (client.available()) {
        Serial.print((char)client.read());
    }
}

void sendValue(int value) {
        pb_ostream_t stream;

        stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

        Value v = Value_init_default;
        v.has_timestamp = false;
        v.which_resource = Value_dumpvalue_tag;
        v.resource.dumpvalue = analogRead(A0);

        Write w = Write_init_default;
        w.values[0] = v;
        w.values_count = 1;
        w.has_activation_code = false;

        if (!pb_encode(&stream, Write_fields, &w)) {
            Serial.println("Failed encoding");
        }
        else {
            POST("/v2/things/" + String(TOKEN), buffer, stream.bytes_written);
        }
}

void POST(String url, byte *data, unsigned int length) {
    client.print("POST " + url + " HTTP/1.1\n");
    client.print("Host: protobuf.thethings.io\n");
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
