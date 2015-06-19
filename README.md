# Arduino

Arduino library and examples to connect to http://thethings.iO

## Library Reference

Before using this library, the Ethernet should be initialized with `Ethernet.begin()`.

```Arduino
TheThings();
TheThings(String token);
```

Create a new TheThings object. The first form is used to activate new things with the `activate(String)` method. The second form sets the token for an already activated thing.

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
