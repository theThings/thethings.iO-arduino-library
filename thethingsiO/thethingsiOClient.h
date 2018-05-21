// Copyright (c) 2015 thethingsiOClient.iO
// This file is licensed under the MIT License
// http://opensource.org/licenses/MIT

#ifndef TEHTHINGSCLIENT_H
#define TEHTHINGSCLIENT_H

#include <Arduino.h>
#include <Client.h>

class thethingsiOClient {
public:
    thethingsiOClient(Client *, Client *);
    thethingsiOClient(Client *, Client *, String &);

    // Create a new thing passing it's existing token as an argument or leave
    // it empty to activate it later using "activate" function.
    String activate(String productId, String activationCode);

    // Return thing's token
    String getToken();

    // Set the thing token
    void setToken(String &);

    // To write a variable into the theThings.iO cloud, call this function with
    // the value to write. This function can be called any times to add more
    // variables to be sent. Finally, call the "send" function to actually
    // write the values.

    // key: string
    // value: string
    void addValue(String key, String value, double lon = 999, double lat = 999);

    // key: string
    // value: number
    void addValue(String key, double value, double lon = 999, double lat = 999);

    // Actually send the values to theThings.iO. See function "addValue".
    String send();

    // Read a variable from the theThings.iO. If only the argument "key" is
    // specified, the last value will be returned. This function will return
    // "limit" number of values of the variable inside an array.
    //
    // key: name of the variable
    // limit: max number of values to return.
    // startDate and endDate format: YYYYMMDDHHmmss
    String read(String);
    String read(String, int);
    
    //Costum Function to Read Time from Server
    String readTime(String);
    ////
		
    void subscribe(int const keep_alive = 20000);
    bool subscribed();
    int available();
    String read();

protected:
    String token;
    String data;
    Client *regular_client;
    Client *subscription_client;
    //static byte server[];
    static char server[];

    // Private functions
    bool POST(Client *, String, String &);
    bool GET(Client *, String &);
    bool isGeo(double, double);
    String geoJSON(double, double);
};

#endif // TEHTHINGSCLIENT_H
