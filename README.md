# Arduino

Arduino library and examples to connect to http://thethings.iO

## Installation

Clone this repository and copy or link the TheTings folder in your arduino library folder

## Library Reference

If you want to use the Ethernet client you should `#include <SPI.h>` `#include <Ethernet.h>` before including this library. If you want to use the GSM client do `#include <GSM.h>`. Then include this library with `#include <thethingsiO.h>`. Before using any client from this library, the Ethernet or GSM shield should be initialized.

```Arduino
// Initialize Ethernet
while (Ethernet.begin(mac) == 0)
    Serial.println("DHCP Failed, retrying");
Serial.println("Arduino connected to network using DHCP");
```

```Arduino
// Initialize GSM
while (!((gsmAccess.begin(PINNUMBER) == GSM_READY) & (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY)))
    Serial.println("Connection failed, retrying");
Serial.println("Arduino connected to network");
```

```Arduino
thethingsiOEthernet();
thethingsiOGSM();

thethingsiOEthernet(String token);
thethingsiOGSM(String token);
```

Create a new client. The first form is used to activate new things with the `activate(String)` method. The second form sets the token for an already activated thing.

```Arduino
String activate(String activationCode);
```

Activates a thing with the given activation code. The token for the Thing is automatically set and returned. In the case of failure an empty string is returned.

```Arduino
String getToken();
```

Returns the token associated with this thing.

```Arduino
void addValue(String key, String value);
void addValue(String key, double value);
```

Adds the key/value pair in the buffer of data to be sent. Since those pairs are stored in memory until the data is sent the user should be aware of the memory.

```Arduino
String send();
```

Send and flush the data stored in the data buffer (added with the `addValue` functions).

```Arduino
String read(String key);
String read(String key, int limit);
```

Get the last `limit` key/value pairs sent to this thing with the given key. In the first form , the last 10 items are returned.

```Arduino
void subscribe();
int available();
String read();
```

Subscribe, check for data to be read and actually read the data from the thing. This is used to listen and get the data sent to this thing.
