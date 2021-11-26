#include "main.h"
// #include "reedSwitch.h"
// #include "button_arm.h"

#include <Wifi.h>
#include <stdint.h>

void setup() {
    Serial.begin(BAUD_RATE) ;

    pinMode(REED_PIN, INPUT) ;
    pinMode(LED_PIN, OUTPUT) ;

    // wifi_enabled = false ;
    // initWifi() ;

    attachInterrupt(digitalPinToInterrupt(BUTTON), ISR_buttonPressed, RISING) ;
}

void loop() {
    if (button.interruptCounter > 0) {
        portENTER_CRITICAL(&button.mux) ;
        button.interruptCounter-- ;
        portEXIT_CRITICAL(&button.mux) ;

        // Interrupt handling code
        button.totalInterruptCounter++ ;
        Serial.printf("A button interrupt has occurred. Total Number: %u\n", button.totalInterruptCounter) ;
        if ( (button.totalInterruptCounter) && (button.totalInterruptCounter % 2) ) {
            // Enable wifi flag

            // Enable reedSwitch checking
            checkReedState() ;
        }
    }
    #ifdef later
    // Figure out a way to enable wifi once and only o
    // Make it so that this is a wakeup from sleep
    if (wifiEnable) {
        initWifi() ;
        wifiEnable = false ;
    }
    #endif
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* FUNCTION PROTOTYPES */

void checkReedState() {
    Reed_State_t    reedState ;
    LED_State_t     ledState ;

    reedState = (Reed_State_t)( digitalRead(REED_PIN) ) ;

    if (reedState != REED_CLOSED) {                                             // LED turns on if reed isn't closed
        ledState = LED_ON ;
        digitalWrite(LED_PIN, ledState) ;
    }
    else {
        ledState = LED_OFF ;
        digitalWrite(LED_PIN, ledState) ;
    }

    delay(READ_DELAY) ;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void IRAM_ATTR ISR_buttonPressed(void) {
    portENTER_CRITICAL_ISR(&button.mux) ;
    button.interruptCounter++ ;
    portEXIT_CRITICAL_ISR(&button.mux) ;
}

