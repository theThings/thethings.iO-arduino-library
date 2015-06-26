#ifndef THETHINGSGSM_H
#define THETHINGSGSM_H

#include <GSM.h>
#include "TheThingsClient.h"

class TheThingsGSM : public TheThingsClient {
public:
    TheThingsGSM() : TheThingsClient(&regular_client, &subscription_client) { }
    TheThingsGSM(String token) : TheThingsClient(&regular_client, &subscription_client, token) { }
private:
    GSMClient regular_client;
    GSMClient subscription_client;
};

#endif // THETHINGSGSM_H
