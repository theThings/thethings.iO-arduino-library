#ifndef THETHINGSGSM_H
#define THETHINGSGSM_H

#include <GSM.h>
#include "thethingsiOClient.h"

class thethingsiOGSM : public thethingsiOClient {
public:
    thethingsiOGSM() : thethingsiOClient(&regular_client, &subscription_client) { }
    thethingsiOGSM(String token) : thethingsiOClient(&regular_client, &subscription_client, token) { }
private:
    GSMClient regular_client;
    GSMClient subscription_client;
};

#endif // THETHINGSGSM_H
