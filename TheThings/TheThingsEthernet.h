#ifndef THETHINGSETHERNET_H
#define THETHINGSETHERNET_H

#include <Ethernet.h>
#include "TheThingsClient.h"

class TheThingsEthernet : public TheThingsClient {
public:
    TheThingsEthernet() : TheThingsClient(&regular_client, &subscription_client) { }
    TheThingsEthernet(String token) : TheThingsClient(&regular_client, &subscription_client, token) { }
private:
    EthernetClient regular_client;
    EthernetClient subscription_client;
};

#endif // THETHINGSETHERNET_H
