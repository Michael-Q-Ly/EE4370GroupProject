#include "main.h"
// #include "reedSwitch.h"
// #include "button_arm.h"

#include <Wifi.h>
#include <stdint.h>

void setup() {
    Serial.begin(BAUD_RATE) ;

    pinMode(BUTTON,     INPUT_PULLUP) ;
    pinMode(REED_PIN,   INPUT_PULLUP) ;
    pinMode(LED_PIN,    OUTPUT) ;
        
    Serial.println("Enabling light sleep mode...") ;
    light_sleep_button() ;
    Serial.println("Alarm is armed.") ;
    light_sleep_switch() ;
    Serial.println("WARNING: ALARM HAS TRIPPED!") ;
    
    Serial.println("Connecting to WiFi...") ;
    init_Wifi() ;
    
    Serial.println("Going to deep sleep mode...") ;
    deep_sleep() ;


}

void loop() {
    // Do nothing
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* FUNCTION PROTOTYPES */

/* Interrupt service request for button */
// void IRAM_ATTR ISR_buttonPressed(void) {
//     portENTER_CRITICAL_ISR(&button.mux) ;
//     button.interruptCounter++ ;
//     portEXIT_CRITICAL_ISR(&button.mux) ;
// }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Interrupt service request for button */
// void IRAM_ATTR ISR_reedOpen(void) {
//     portENTER_CRITICAL_ISR(&reed.mux) ;
//     reed.interruptCounter++ ;
//     portEXIT_CRITICAL_ISR(&reed.mux) ;
// }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Reads the digital pin of the reed switch and turns an LED on if it is open */
// void checkReedState(void) {
//     // Reed_State_t    reedState ;
//     // LED_State_t     ledState ;

//     reedState = (Reed_State_t)( digitalRead(REED_PIN) ) ;

//     if (reedState != REED_CLOSED) {                                             // LED turns on if reed isn't closed
//         ledState = LED_ON ;
//         digitalWrite(LED_PIN, ledState) ;
//         printf("WARNING: Door is open!") ;
//     }
//     else {
//         ledState = LED_OFF ;
//         digitalWrite(LED_PIN, ledState) ;
//     }

//     delay(READ_DELAY) ;
// }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void init_Wifi(void) {
    uint8_t tries;
    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA) ;
    WiFi.disconnect() ;
    delay(100) ;

    WiFi.begin(SSID, PASS) ;
    tries = 0 ;
    while ( (WiFi.status() != WL_CONNECTED) || (++tries < MAX_WIFI_ATTEMPTS) ) {
        Serial.printf(".") ;
        delay(1000) ;
    }
    Serial.printf("You are now connected to ") ;
    Serial.println( WiFi.localIP() ) ;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void deep_sleep(void) {
    esp_sleep_enable_ext0_wakeup(REED_PIN, REED_OPEN) ;                                      // Configures deep sleep wakeup sources (GPIO)
                                                                                            // then puts the ESP32 into deep sleep mode.
    esp_deep_sleep_start() ;                                                                // Prints wakeup reason when woken up.
    Serial.println("Enabling deep sleep mode...") ;
    wakeup_reason = esp_sleep_get_wakeup_cause() ;
    switch(wakeup_reason)
    {
        case ESP_SLEEP_WAKEUP_EXT0      : Serial.println("Wakeup caused by external signal using RTC_IO") ;             break ;
        case ESP_SLEEP_WAKEUP_EXT1      : Serial.println("Wakeup caused by external signal using RTC_CNTL") ;           break ;
        case ESP_SLEEP_WAKEUP_TIMER     : Serial.println("Wakeup caused by timer") ;                                    break ;
        case ESP_SLEEP_WAKEUP_TOUCHPAD  : Serial.println("Wakeup caused by touchpad") ;                                 break ;
        case ESP_SLEEP_WAKEUP_ULP       : Serial.println("Wakeup caused by ULP program") ;                              break ;
        default                         : Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason) ;   break ;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void light_sleep_button(void) {                                                                   // Put the device in light sleep mode.
    esp_sleep_enable_ext0_wakeup(BUTTON, BUTTON_ON) ;                                      // Configures light sleep wakeup sources (GPIO)
                                                                                            // then puts the ESP32 into light sleep mode.
    esp_light_sleep_start() ;                                                               // Prints wakeup reason when woken up.

    wakeup_reason = esp_sleep_get_wakeup_cause() ;
    switch(wakeup_reason)
    {
        case ESP_SLEEP_WAKEUP_EXT0      : Serial.println("Wakeup caused by external signal using RTC_IO") ;             break ;
        case ESP_SLEEP_WAKEUP_EXT1      : Serial.println("Wakeup caused by external signal using RTC_CNTL") ;           break ;
        case ESP_SLEEP_WAKEUP_TIMER     : Serial.println("Wakeup caused by timer") ;                                    break ;
        case ESP_SLEEP_WAKEUP_TOUCHPAD  : Serial.println("Wakeup caused by touchpad") ;                                 break ;
        case ESP_SLEEP_WAKEUP_ULP       : Serial.println("Wakeup caused by ULP program") ;                              break ;
        default                         : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason) ;    break ;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void light_sleep_switch(void) {                                                                   // Put the device in light sleep mode.
    esp_sleep_enable_ext0_wakeup(REED_PIN, REED_OPEN) ;                                      // Configures light sleep wakeup sources (GPIO)
                                                                                            // then puts the ESP32 into light sleep mode.
    esp_light_sleep_start() ;                                                               // Prints wakeup reason when woken up.

    wakeup_reason = esp_sleep_get_wakeup_cause() ;
    switch(wakeup_reason)
    {
        case ESP_SLEEP_WAKEUP_EXT0      : Serial.println("Wakeup caused by external signal using RTC_IO") ;             break ;
        case ESP_SLEEP_WAKEUP_EXT1      : Serial.println("Wakeup caused by external signal using RTC_CNTL") ;           break ;
        case ESP_SLEEP_WAKEUP_TIMER     : Serial.println("Wakeup caused by timer") ;                                    break ;
        case ESP_SLEEP_WAKEUP_TOUCHPAD  : Serial.println("Wakeup caused by touchpad") ;                                 break ;
        case ESP_SLEEP_WAKEUP_ULP       : Serial.println("Wakeup caused by ULP program") ;                              break ;
        default                         : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason) ;    break ;
    }
}