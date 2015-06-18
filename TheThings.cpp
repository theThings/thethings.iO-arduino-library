#include "TheThings.h"

#define TOKEN_SIZE 43

byte TheThings::server[] = {77, 73, 82, 243};

TheThings::TheThings() {
    this->token = "";
    this->data = "";
}

TheThings::TheThings(String token) {
    this->token = token;
    this->data = "";
}

String TheThings::activate(String activationCode) {
    token = "";
    String body = "";
    String find_token = "thingToken\":\"";
    int start;
    char c;
    if (POST(client, "/v2/things/", "{ \"activationCode\":\"" + activationCode + "\"}")) {
        delay(1000);
        while (client.available() && (c = (char)client.read()) != '{');
        if (c == '{') {
            while (client.available() && (c = (char)client.read()) != '\n') body.concat(c);
            if ((start = body.indexOf(find_token)) >= 0) {
                start = start + find_token.length();
                token = body.substring(start, start + TOKEN_SIZE);
            }
        }
    }
    return token;
}

String TheThings::getToken() {
    return token;
}

void TheThings::addValue(String key, String value) {
    data.concat("{\"key\":\"" + key + "\",\"value\":\"" + value + "\"}");
}

void TheThings::addValue(String key, double value) {
    addValue(key, String(value));
}

String TheThings::send() {
    String received = "";
    if (POST(client, "/v2/things/" + token, "{\"values\":[" + data + "]}")) {
        data = "";
        delay(1000);
        while (client.available()) {
            char c = (char)client.read();
            received.concat(c);
        }
    }
    return received;
}

bool TheThings::POST(EthernetClient &client, String url, String &data) {
    client.flush();
    client.stop();
    if (client.connected() || client.connect(server, 80)) {
        client.print("POST " + url + " HTTP/1.1\n");
        client.print("Host: api.thethings.io\n");
        client.print("Connection: close\n");
        client.print("Content-Type: application/json\n");
        client.print("Content-Length: ");
        client.print(data.length());
        client.print("\n\n");
        client.print(data);
    }
    return client.connected();
}

bool TheThings::GET(EthernetClient &client, String &data) {
    client.flush();
    if (client.connected() || client.connect(server, 80)) {
        client.print("GET " + data + " HTTP/1.1\n");
        client.print("Host: api.thethings.io\n");
        client.print("Accept: application/json\n\n");
    }
    return client.connected();
}

String TheThings::read(String key) {
    return read(key, 10);
}

String TheThings::read(String key, int limit) {
    String received = "";
    if (client.connect(server, 80)) {
        GET(client, "/v2/things/" + token + "/resources/" + key + "?limit=" + String(limit));
        delay(1000);
        while (client.available())
            received.concat((char)client.read());
        client.stop();
    }
    return received;
}

void TheThings::subscribe() {
    subscription_client.stop();
    GET(subscription_client, "/v2/things/" + token);
}

int TheThings::available() {
    return subscription_client.available();
}

String TheThings::read() {
    String received = "";
    if (subscription_client.connected()) {
        while (subscription_client.available())
            received.concat((char)subscription_client.read());
    }
    return received;
}
