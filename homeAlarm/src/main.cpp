#include "main.h"
// #include "reedSwitch.h"
// #include "button_arm.h"

#include <Wifi.h>
#include <stdint.h>

void setup() {
    Serial.begin(BAUD_RATE) ;

    pinMode(REED_PIN, INPUT) ;
    pinMode(LED_PIN, OUTPUT) ;

    wifiEnable = false ;
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
        if ( (button.totalInterruptCounter) && (button.totalInterruptCounter % 2) ) {   // Anything but zero and even presses
            // Enable wifi flag
            wifiEnable = true ;
            // Enable reedSwitch checking
            checkReedState() ;
        }
        else {
            // Deep sleep mode
        }
        if (wifiEnable) {
            initWifi() ;
            wifiEnable = false ;
        }
    }
    #ifdef later
    // Figure out a way to enable wifi once and only o
    // Make it so that this is a wakeup from sleep
    #endif
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* FUNCTION PROTOTYPES */

/* Interrupt service request for button */
void IRAM_ATTR ISR_buttonPressed(void) {
    portENTER_CRITICAL_ISR(&button.mux) ;
    button.interruptCounter++ ;
    portEXIT_CRITICAL_ISR(&button.mux) ;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Reads the digital pin of the reed switch and turns an LED on if it is open */
void checkReedState(void) {
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

void initWifi(void) {
    uint8_t tries;
    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA) ;
    WiFi.disconnect() ;
    delay(100) ;

    WiFi.begin(SSID, PASS) ;
    Serial.printf("Connection to WiFi...\n") ;
    tries = 0 ;
    while ( (WiFi.status() != WL_CONNECTED) || (++tries < 20) ) {
        Serial.printf(".") ;
        delay(1000) ;
    }
    Serial.printf("You are now connected to ") ;
    Serial.println( WiFi.localIP() ) ;
}