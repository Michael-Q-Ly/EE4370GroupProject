#include "main.h"
#include "reedSwitch.h"

/*
 * Reads the digital pin of the reed switch
 */
void readDigitalVal() {
    int reedVal ;

    reedVal = digitalRead(REED_PIN) ;

    if (reedVal != HIGH) {
        digitalWrite(LED_PIN, HIGH) ;
    }
    else {
        digitalWrite(LED_PIN, LOW) ;
    }

    delay(READ_DELAY) ;
}