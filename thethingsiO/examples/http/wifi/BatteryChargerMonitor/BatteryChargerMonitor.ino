#include <SoftwareSerial.h>

// theThings.io IP address to which de data is sent
#define IP "77.73.82.243"

// WiFi information to connect to
#define SSID YOURSSID
#define PASS YOURPASSWORD

// Thing Token
#define THING YOURTHINGID

// Part of the header to send to theThings.io
String POST = "POST /v2/things/" + String(THING) + " HTTP/1.1\nHost: api.thethings.io\nAccept: application/json\nContent-Type: application/json\n";

SoftwareSerial wifi(3, 2);

struct battery {
    int state;
    int pin;
};

#define BATTERIES 4

struct battery batteries[BATTERIES];

void setup() {
    for (int i = 0; i < BATTERIES; i++) {
        batteries[i].pin = i;
        batteries[i].state = 0;
    }
    Serial.begin(9600);
    wifi.begin(9600);
    while (!connectWiFi()) Serial.println("Connecting to WiFi...");
    Serial.println("Connected to WiFi");
}

void loop() {
    updateStatus();
    uploadStatus();
    delay(7000);
}

// Scan the *BATTERIES* analog inputs to get and save the state of each battery
void updateStatus() {
    for (int i = 0; i < BATTERIES; i++) {
        // Since 0-5V is 0-1023, a map from read value is required to represent
        // the voltage in %.
        batteries[i].state = map(analogRead(batteries[i].pin), 0, 300, 0, 100);
        Serial.println(batteries[i].state);
    }
}

// Send the data read from the analog inputs to the server storing the data.
void uploadStatus() {
    String cmd = "AT+CIPSTART=\"TCP\",\"" + String(IP) + "\",80";
    wifi.println(cmd);

    delay(2000);

    if (wifi.find("Error")) return;

    // Iterate over each input and add it to the values list
    // Each Battery has it's key "BATT#" where # is the pin number.
    String data = "{\"values\":[";
    String comma = "";
    for (int i = 0; i < BATTERIES; i++) {
        data += comma + "{\"key\":\"BATT" + String(i) + "\",\"value\":\"" + String(batteries[i].state) + "\"}";
        if (comma == "") comma = ",";
    }
    data += "]}";
    cmd = POST + "Content-Length: " + data.length() + "\n\n" + data + "\r\n";

    Serial.println(cmd);

    wifi.print("AT+CIPSEND=");
    wifi.println(cmd.length());
    if (wifi.find(">")) wifi.print(cmd);
    else wifi.println("AT+CIPCLOSE");
}

// Configure the WiFi shield with the network information and try to connect
boolean connectWiFi() {
    wifi.println("AT+CWMODE=3");
    delay(2000);
    String cmd = "AT+CWJAP=\"" + String(SSID) + "\",\"" + String(PASS) + "\"";
    wifi.println(cmd);
    delay(5000);
    if (wifi.find("OK")) return true;
    else return false;
}

