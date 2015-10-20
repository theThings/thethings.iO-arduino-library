#ifndef THETHINGSWIFI_H
#define THETHINGSWIFI_H

#include "thethingsiOClient.h"

class thethingsiOWiFi : public thethingsiOClient {
public:
    thethingsiOWiFi() : thethingsiOClient(&regular_client, &subscription_client) { }
    thethingsiOWiFi(String token) : thethingsiOClient(&regular_client, &subscription_client, token) { }
private:
    WiFiClient regular_client;
    WiFiClient subscription_client;
};

#endif // THETHINGSWIFI_H
