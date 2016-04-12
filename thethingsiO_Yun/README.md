# Arduino Yun MQTT example

Arduino Yun MQTT example

## Installation

Clone this repository and/or copy the code to your arduino IDE.

## Code Reference
Necessary imports. Process is only used in case you want to see the wifi status with the commented code in the loop function.
```Arduino
#include <YunClient.h>
#include <Bridge.h>
#include <PubSubClient.h>
#include <Process.h>
```
Add your thing token from your thing product on thethings.iO. 
```Arduino
#define TOKEN "yourtoken"

YunClient wificlient;

String topic = "v2/things/" + String(TOKEN);
String message = "";
boolean firstValue = true;
```
Functions to check if the GPS coordinates are correct and to create a valid JSON string with them.
```Arduino
bool isGeo(double lon, double lat) {
    return lon >= -180  && lon <= 180 && lat >= -90 && lat >= 90;
}

String geoJSON(double lon, double lat) {
    return "\"geo\":{\"lat\":" + String(lat, 7) + ",\"long\":" + String(lon, 7) + "}";
}
```
AddValue function. Adds the value to an associated key (with coordinates) and it creates the necessary JSON.  Call addValue before publish in order to send the values to thethings.iO.
```Arduino
void addValue(String key, int value, double lat, double lon) {
  if (firstValue == true) {
    firstValue = false;
    message.concat("{\"key\":\"" + key + "\",\"value\":" + value +(isGeo(lon, lat) ? "," + geoJSON(lon, lat) : "")+"}");
  }
  else {
    message.concat(",{\"key\":\"" + key + "\",\"value\":" + value +(isGeo(lon, lat) ? "," + geoJSON(lon, lat) : "") +"}");
  }
}
```
AddValue function. Adds the value to an associated key (without coordinates) and it creates the necessary JSON. Can be called more than once (taking into account the maximum of 90 bytes specified by the pubsubclient). Call addValue before publish in order to send the values to thethings.iO.
```Arduino
void addValue(String key, int value) {
  if (firstValue == true) {
    firstValue = false;
    message.concat("{\"key\":\"" + key + "\",\"value\":" + value+"}");
  }
  else {
    message.concat(",{\"key\":\"" + key + "\",\"value\":" + value+"}");
  }
}
```
Callback function that is associated with the mqtt client. This function is called each time a messages is sent/received with this topic. The default behaviour is print the received/sent message;
```Arduino
void callback(char* topic, byte* payload, unsigned int length) {
    // handle message arrived
    String text = "";
    for (int i = 0; i < length; i++)
        text.concat((char)payload[i]);
    text.replace(" ", "");
    Serial.println(text);
}
```
Declaration of the mqtt pubsubclient, with thethings.iO as the server and using the wifi YunClient;
```Arduino
PubSubClient mqtt("mqtt.thethings.io", 1883, callback, wificlient);
```

Publish function: Sends all the values and the keys (with a maxim of 90 bytes specified by the pubsubclient library).
```Arduino
void publish() {
  String toSend = "{\"values\":[" + message + "]}";
  mqtt.publish((char*)topic.c_str(), (char*)toSend.c_str());
  message= "";
  firstValue = true;
  Serial.println("Published");
}
```
Setup function, that enables the serial communications, the bridge (necessary for the Yun) and connects to the thethings.iO server via mqtt. Later on, it subscribes to the same topic that sends.
```Arduino
void setup() {
  Serial.begin(9600); 
  while(!Serial);
  Bridge.begin();
  while (!mqtt.connect("Yunclient")) {
    Serial.println("Could not subscribe, retrying...");
  }
  Serial.println("Client connected to mqtt broker!");
  mqtt.subscribe((char*)topic.c_str());
}
```
Loop function that repeats. If you want to see your wifi status, uncomment the block.

```Arduino
void loop() {
 /* Process wifiCheck;
  wifiCheck.runShellCommand("/usr/bin/pretty-wifi-info.lua");
  while (wifiCheck.available() > 0) {
    char c = wifiCheck.read();
    Serial.print(c);
  }

  Serial.println();

  delay(000);
  // put your main code here, to run repeatedly:
  */
  mqtt.loop();
}
```