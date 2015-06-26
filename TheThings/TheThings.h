#ifndef THETHINGS_H
#define THETHINGS_H

#include "TheThingsClient.h"

#if defined(ethernet_h)
#include "TheThingsEthernet.h"
#endif

#if defined(_GSM3SIMPLIFIERFILE_)
#include "TheThingsGSM.h"
#endif

#endif // THETHINGS_H
