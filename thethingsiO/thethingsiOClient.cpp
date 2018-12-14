#include "thethingsiOClient.h"

#define TOKEN_SIZE 43

//byte thethingsiOClient::server[] = {77, 73, 82, 243};
char thethingsiOClient::server[] = "api.devices.thethings.io";

thethingsiOClient::thethingsiOClient(Client *regular, Client *subscription)
{
    this->regular_client = regular;
    this->subscription_client = subscription;
    this->token = "";
    this->data = "";
}

thethingsiOClient::thethingsiOClient(Client *regular, Client *subscription, String &token)
{
    this->regular_client = regular;
    this->subscription_client = subscription;
    this->token = token;
    this->data = "";
}

String thethingsiOClient::activate(String productId, String activationCode)
{
    token = "";
    String body = "";
    String find_token = "thingToken\":\"";
    int start;
    char c;
    if (POST(regular_client, "/v2/things/", "{\"activationCode\":\"" + activationCode + "\",\"productId\":\"" + productId + "\"}"))
    {
        delay(1000);
        while (regular_client->available() && (c = (char)regular_client->read()) != '{')
            ;
        if (c == '{')
        {
            while (regular_client->available() && (c = (char)regular_client->read()) != '\n')
                body.concat(c);
            if ((start = body.indexOf(find_token)) >= 0)
            {
                start = start + find_token.length();
                token = body.substring(start, start + TOKEN_SIZE);
            }
        }
    }
    return token;
}

String thethingsiOClient::getToken()
{
    return token;
}

void thethingsiOClient::setToken(String &token)
{
    this->token = token;
}

bool thethingsiOClient::isGeo(double lon, double lat)
{
    return lon >= -180 && lon <= 180 && lat >= -90 && lat <= 90;
}

String thethingsiOClient::geoJSON(double lon, double lat)
{
    return "\"geo\":{\"lat\":" + String(lat, 7) + ",\"long\":" + String(lon, 7) + "}";
}

void thethingsiOClient::addValue(String key, String value, double lon, double lat)
{
    data.concat(
        String(data == "" ? "" : ",") + "{\"key\":\"" + key + "\",\"value\":\"" + value + "\"" + (isGeo(lon, lat) ? "," + geoJSON(lon, lat) : "") + "}");
}

void thethingsiOClient::addValue(String key, double value, double lon, double lat)
{
    data.concat(
        String(data == "" ? "" : ",") + "{\"key\":\"" + key + "\",\"value\":\"" + value + "\"" + (isGeo(lon, lat) ? "," + geoJSON(lon, lat) : "") + "}");
}

String thethingsiOClient::send()
{
    String received = "";
    if (POST(regular_client, "/v2/things/" + token, "{\"values\":[" + data + "]}"))
    {
        delay(1000);
        while (regular_client->available())
        {
            char c = (char)regular_client->read();
            received.concat(c);
        }
    }
    data = "";
    return received;
}

bool thethingsiOClient::POST(Client *client, String url, String &data)
{
    client->flush();
    client->stop();
    if (client->connected() || client->connect(server, 80))
    {
        client->print("POST " + url + " HTTP/1.1\n");
        client->print("Host: api.devices.thethings.io\n");
        client->print("Connection: close\n");
        client->print("Content-Type: application/json\n");
        client->print("Content-Length: ");
        client->print(data.length());
        client->print("\n\n");
        client->print(data);
    }
    return client->connected();
}

bool thethingsiOClient::GET(Client *client, String &data)
{
    client->flush();
    if (client->connected() || client->connect(server, 80))
    {
        client->print("GET " + data + " HTTP/1.1\n");
        client->print("Host: api.devices.thethings.io\n");
        client->print("Accept: application/json\n\n");
    }
    return client->connected();
}

String thethingsiOClient::read(String key)
{
    return read(key, 10);
}

String thethingsiOClient::read(String key, int limit)
{
    String received = "";
    if (regular_client->connect(server, 80))
    {
        GET(regular_client, "/v2/things/" + token + "/resources/" + key + "?limit=" + String(limit));
        delay(1000);
        while (regular_client->available())
            received.concat((char)regular_client->read());
        regular_client->stop();
    }
    return received;
}

//Costum Function To Retrieve Date Time From Server
String thethingsiOClient::readTime(String format)
{
    int start;
    int end;
    String received = "";

    if (regular_client->connect(server, 80))
    {
        GET(regular_client, "/v2/utils/date?thingToken=" + token + "&format=" + format);
        delay(1000);
        while (regular_client->available())
            received.concat((char)regular_client->read());
        regular_client->stop();
    }

    //Retrieve the Time from Response
    String dummy = "value";
    dummy.concat('"');
    dummy.concat(':');
    start = received.lastIndexOf(dummy) + dummy.length();
    end = received.lastIndexOf("]") - 1;
    received = received.substring(start, end);

    //
    return received;
}

void thethingsiOClient::subscribe(int const keep_alive)
{
    subscription_client->stop();
    GET(subscription_client, "/v2/things/" + token + "?keepAlive=" + String(keep_alive));
}

bool thethingsiOClient::subscribed()
{
    return subscription_client->connected();
}

int thethingsiOClient::available()
{
    return subscription_client->available();
}

String thethingsiOClient::read()
{
    String received = "";
    if (subscription_client->connected())
    {
        while (subscription_client->available())
            received.concat((char)subscription_client->read());
    }
    return received;
}
