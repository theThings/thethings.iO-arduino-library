#include <GSM.h>
#include <thethingsiO.h>

#define TOKEN "your token here"

// PIN Number
#define PINNUMBER ""

// APN data
#define GPRS_APN "m2mkit.telefonica.com"
#define GPRS_LOGIN ""
#define GPRS_PASSWORD ""

int const pin_led = 10;
int count = 0;

thethingsiOGSM thing(TOKEN);

GPRS gprs;
GSM gsmAccess;

int const button = 10;
bool lastPressed = false;

void setup() {
    Serial.begin(9600);
    pinMode(button, OUTPUT);

    startGSM();
}

void loop() {
    if (digitalRead(button) == LOW) {
        if (!lastPressed) {
            Serial.println("PUSH!");
            thing.addValue("Button", count++);
            Serial.println(thing.send());
            delay(100);
            lastPressed = true;
        }
    }
    else {
        lastPressed = false;
    }
}

void startGSM() {
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
