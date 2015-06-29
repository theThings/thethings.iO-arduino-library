#ifndef THETHINGSETHERNET_H
#define THETHINGSETHERNET_H

#include <Ethernet.h>
#include "thethingsiOClient.h"

class thethingsiOEthernet : public thethingsiOClient {
public:
    thethingsiOEthernet() : thethingsiOClient(&regular_client, &subscription_client) { }
    thethingsiOEthernet(String token) : thethingsiOClient(&regular_client, &subscription_client, token) { }
private:
    EthernetClient regular_client;
    EthernetClient subscription_client;
};

#endif // THETHINGSETHERNET_H
