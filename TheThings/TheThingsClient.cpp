#include "TheThingsClient.h"

#define TOKEN_SIZE 43

byte TheThingsClient::server[] = {77, 73, 82, 243};

TheThingsClient::TheThingsClient(Client *regular, Client *subscription) {
    this->regular_client = regular;
    this->subscription_client = subscription;
    this->token = "";
    this->data = "";
}

TheThingsClient::TheThingsClient(Client *regular, Client *subscription, String &token) {
    this->regular_client = regular;
    this->subscription_client = subscription;
    this->token = token;
    this->data = "";
}

String TheThingsClient::activate(String activationCode) {
    token = "";
    String body = "";
    String find_token = "thingToken\":\"";
    int start;
    char c;
    if (POST(regular_client, "/v2/things/", "{ \"activationCode\":\"" + activationCode + "\"}")) {
        delay(1000);
        while (regular_client->available() && (c = (char)regular_client->read()) != '{');
        if (c == '{') {
            while (regular_client->available() && (c = (char)regular_client->read()) != '\n') body.concat(c);
            if ((start = body.indexOf(find_token)) >= 0) {
                start = start + find_token.length();
                token = body.substring(start, start + TOKEN_SIZE);
            }
        }
    }
    return token;
}

String TheThingsClient::getToken() {
    return token;
}

void TheThingsClient::addValue(String key, String value) {
    data.concat("{\"key\":\"" + key + "\",\"value\":\"" + value + "\"}");
}

void TheThingsClient::addValue(String key, double value) {
    addValue(key, String(value));
}

String TheThingsClient::send() {
    String received = "";
    if (POST(regular_client, "/v2/things/" + token, "{\"values\":[" + data + "]}")) {
        data = "";
        delay(1000);
        while (regular_client->available()) {
            char c = (char)regular_client->read();
            received.concat(c);
        }
    }
    return received;
}

bool TheThingsClient::POST(Client *client, String url, String &data) {
    client->flush();
    client->stop();
    if (client->connected() || client->connect(server, 80)) {
        client->print("POST " + url + " HTTP/1.1\n");
        client->print("Host: api.thethings.io\n");
        client->print("Connection: close\n");
        client->print("Content-Type: application/json\n");
        client->print("Content-Length: ");
        client->print(data.length());
        client->print("\n\n");
        client->print(data);
    }
    return client->connected();
}

bool TheThingsClient::GET(Client *client, String &data) {
    client->flush();
    if (client->connected() || client->connect(server, 80)) {
        client->print("GET " + data + " HTTP/1.1\n");
        client->print("Host: api.thethings.io\n");
        client->print("Accept: application/json\n\n");
    }
    return client->connected();
}

String TheThingsClient::read(String key) {
    return read(key, 10);
}

String TheThingsClient::read(String key, int limit) {
    String received = "";
    if (regular_client->connect(server, 80)) {
        GET(regular_client, "/v2/things/" + token + "/resources/" + key + "?limit=" + String(limit));
        delay(1000);
        while (regular_client->available())
            received.concat((char)regular_client->read());
        regular_client->stop();
    }
    return received;
}

void TheThingsClient::subscribe() {
    subscription_client->stop();
    GET(subscription_client, "/v2/things/" + token);
}

bool TheThingsClient::subscribed() {
    return subscription_client->connected();
}

int TheThingsClient::available() {
    return subscription_client->available();
}

String TheThingsClient::read() {
    String received = "";
    if (subscription_client->connected()) {
        while (subscription_client->available())
            received.concat((char)subscription_client->read());
    }
    return received;
}
