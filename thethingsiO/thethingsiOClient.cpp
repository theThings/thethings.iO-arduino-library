#include "thethingsiOClient.h"

#define TOKEN_SIZE 43

byte thethingsiOClient::server[] = {77, 73, 82, 243};

thethingsiOClient::thethingsiOClient(Client *regular, Client *subscription) {
    this->regular_client = regular;
    this->subscription_client = subscription;
    this->token = "";
    this->data = "";
}

thethingsiOClient::thethingsiOClient(Client *regular, Client *subscription, String &token) {
    this->regular_client = regular;
    this->subscription_client = subscription;
    this->token = token;
    this->data = "";
}

String thethingsiOClient::activate(String activationCode) {
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

String thethingsiOClient::getToken() {
    return token;
}

void thethingsiOClient::addValue(String key, String value) {
    data.concat("{\"key\":\"" + key + "\",\"value\":\"" + value + "\"}");
}

void thethingsiOClient::addValue(String key, double value) {
    addValue(key, String(value));
}

String thethingsiOClient::send() {
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

bool thethingsiOClient::POST(Client *client, String url, String &data) {
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

bool thethingsiOClient::GET(Client *client, String &data) {
    client->flush();
    if (client->connected() || client->connect(server, 80)) {
        client->print("GET " + data + " HTTP/1.1\n");
        client->print("Host: api.thethings.io\n");
        client->print("Accept: application/json\n\n");
    }
    return client->connected();
}

String thethingsiOClient::read(String key) {
    return read(key, 10);
}

String thethingsiOClient::read(String key, int limit) {
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

void thethingsiOClient::subscribe(int const keep_alive) {
    subscription_client->stop();
    GET(subscription_client, "/v2/things/" + token + "?keepAlive=" + String(keep_alive));
}

bool thethingsiOClient::subscribed() {
    return subscription_client->connected();
}

int thethingsiOClient::available() {
    return subscription_client->available();
}

String thethingsiOClient::read() {
    String received = "";
    if (subscription_client->connected()) {
        while (subscription_client->available())
            received.concat((char)subscription_client->read());
    }
    return received;
}
