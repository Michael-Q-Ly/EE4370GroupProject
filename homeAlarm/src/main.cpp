#include "main.h"
#include "reedSwitch.h"

extern void readDigitalVal() ;

void setup() {
    Serial.begin(BAUD_RATE) ;

    pinMode(REED_PIN, INPUT) ;
    pinMode(LED_PIN, OUTPUT) ;
}

void loop() {
    readDigitalVal() ;
}