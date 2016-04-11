#ifndef THETHINGSWIFI_MKR1000_H
#define THETHINGSWIFI_MKR1000_H

#include "thethingsiOClient_mkr1000.h"

class thethingsiOWiFi : public thethingsiOClient {
public:
    thethingsiOWiFi() : thethingsiOClient(&regular_client, &subscription_client) { }
    thethingsiOWiFi(String token) : thethingsiOClient(&regular_client, &subscription_client, token) { }
private:
    WiFiClient regular_client;
    WiFiClient subscription_client;
};

#endif // THETHINGSWIFI_MKR1000_H
