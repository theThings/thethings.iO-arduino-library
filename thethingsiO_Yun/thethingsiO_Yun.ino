#include <YunClient.h>
#include <Bridge.h>
#include <PubSubClient.h>
#include <Process.h>

#define TOKEN "yourtoken"

YunClient wificlient;

String topic = "v2/things/" + String(TOKEN);
String message = "";
boolean firstValue = true;

bool isGeo(double lon, double lat) {
    return lon >= -180  && lon <= 180 && lat >= -90 && lat >= 90;
}

String geoJSON(double lon, double lat) {
    return "\"geo\":{\"lat\":" + String(lat, 7) + ",\"long\":" + String(lon, 7) + "}";
}


void addValue(String key, int value, double lat, double lon) {
  if (firstValue == true) {
    firstValue = false;
    message.concat("{\"key\":\"" + key + "\",\"value\":\"" + value + "\"" +(isGeo(lon, lat) ? "," + geoJSON(lon, lat) : "")+"}");
  }
  else {
    message.concat(",{\"key\":\"" + key + "\",\"value\":\"" + value + "\"" +(isGeo(lon, lat) ? "," + geoJSON(lon, lat) : "") +"}");
  }
}

void addValue(String key, int value) {
  if (firstValue == true) {
    firstValue = false;
    message.concat("{\"key\":\"" + key + "\",\"value\":\"" + value+"\"}");
  }
  else {
    message.concat(",{\"key\":\"" + key + "\",\"value\":\"" + value+"\"}");
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
    // handle message arrived
    String text = "";
    for (int i = 0; i < length; i++)
        text.concat((char)payload[i]);
    text.replace(" ", "");
    Serial.println(text);
    
}

PubSubClient mqtt("mqtt.thethings.io", 1883, callback, wificlient);

void publish() {
  String toSend = "{\"values\":[" + message + "]}";
  mqtt.publish((char*)topic.c_str(), (char*)toSend.c_str());
  message= "";
  firstValue = true;
  Serial.println("Published");
}

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
