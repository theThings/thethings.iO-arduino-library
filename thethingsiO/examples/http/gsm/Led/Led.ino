#include <GSM.h>
#include <thethingsiO.h>

#define MAX_LIGHT_TIME 5000
#define TOKEN "your token here"

// PIN Number
#define PINNUMBER ""

// APN data
#define GPRS_APN "m2mkit.telefonica.com"
#define GPRS_LOGIN ""
#define GPRS_PASSWORD ""

int const pin_led = 10;

thethingsiOGSM thing(TOKEN);
GPRS gprs;
GSM gsmAccess;

void setup() {
    // initialize serial communications
    Serial.begin(9600);

    pinMode(pin_led, OUTPUT);

    startGSM();

    thing.subscribe();
}

void loop() {
    checkLed(pin_led);
}

void checkLed(int const led) {
    if (thing.available()) {
        Serial.println("READ!");
        String text = thing.read();
        Serial.println(text);
        text.replace(" ", "");
        text.toUpperCase();
        if (text.indexOf("\"KEY\":\"LED\"") >= 0) {
            if (text.indexOf("\"VALUE\":\"ON\"") >= 0) candle(led, MAX_LIGHT_TIME);
            else if (text.indexOf("\"VALUE\":\"OFF\"") >= 0) digitalWrite(led, LOW);
        }
    }
}

void candle(int const pin, int const time) {
    int time_acc = 0;
    int current_time;
    while (time_acc < time) {
        digitalWrite(pin, HIGH);
        current_time = 500 + random(500);
        time_acc += current_time;
        delay(current_time);

        digitalWrite(pin, LOW);
        current_time = 300 + random(350);
        time_acc += current_time;
        delay(current_time);
    }
}

void startGSM() {
    // connection state
    boolean notConnected = true;

    // Start GSM
    // If your SIM has PIN, pass it as a parameter of begin() in quotes
    while(notConnected) {
        if ((gsmAccess.begin(PINNUMBER) == GSM_READY) & (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY))
            notConnected = false;
        else {
            Serial.println("Not connected");
            delay(1000);
        }
    }
    Serial.println("Connected");
}
