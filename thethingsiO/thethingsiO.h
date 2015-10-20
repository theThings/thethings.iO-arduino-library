#ifndef THETHINGS_H
#define THETHINGS_H

#include "thethingsiOClient.h"

#if defined(ethernet_h)
#include "thethingsiOEthernet.h"
#endif

#if defined(_GSM3SIMPLIFIERFILE_)
#include "thethingsiOGSM.h"
#endif

#if defined(wificlient_h)
#include "thethingsiOWiFi.h"
#endif

#endif // THETHINGS_H
