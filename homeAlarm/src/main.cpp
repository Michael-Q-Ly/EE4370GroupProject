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

    // attachInterrupt(digitalPinToInterrupt(BUTTON), ISR_buttonPressed, RISING) ;
}

void loop() {
    Serial.printf("Door state is %d\n", doorClosed) ;
    delay(100) ;

    if (wifiEnabled) {
        Serial.printf("Wifi on\n") ;
        init_Wifi() ;
        wifiEnabled = false ;
    }

    if (alarmArmed && doorClosed) {
        Serial.println("Alarm is armed. Going to deep sleep mode. Wake by opening door.\n") ;
        delay(100) ;
        doorClosed  = false ;
        wifiEnabled = true ;
        deep_sleep_reed_open() ;
    }
    else if (alarmArmed && !doorClosed) {
        Serial.println("Alarm is armed. Going to deep sleep mode. Wake by closing door.\n") ;
        delay(100) ;
        doorClosed  = true ;
        wifiEnabled = true ;
        deep_sleep_reed_closed() ;
    }
    else {
        Serial.printf("Alarm state is ") ;
        Serial.println(alarmArmed) ;
        Serial.println("Going to deep sleep mode until button press...") ;
        delay(100) ;
        alarmArmed = true ;
        deep_sleep_button() ;
    }

    #ifdef later
    if (buttonInterruptCounter > 0) {
        portENTER_CRITICAL(&buttonMUX) ;
        buttonInterruptCounter-- ;
        portEXIT_CRITICAL(&buttonMUX) ;

        // Interrupt handling code
        buttonTotalInterruptCounter++ ;
        Serial.printf("A button interrupt has occurred. Total Number: %u\n", buttonTotalInterruptCounter) ;
        // alarmArmed = !alarmArmed ;
    }
    #endif  /* later */
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* FUNCTION PROTOTYPES */

/* Interrupt service request for button */
// void IRAM_ATTR ISR_buttonPressed(void) {
//     portENTER_CRITICAL_ISR(&buttonMUX) ;
//     buttonInterruptCounter++ ;
//     // alarmArmed = !alarmArmed ;
//     portEXIT_CRITICAL_ISR(&buttonMUX) ;
// }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Interrupt service request for button */
// void IRAM_ATTR ISR_reedOpen(void) {
//     portENTER_CRITICAL_ISR(&reed.mux) ;
//     reed.interruptCounter++ ;
//     portEXIT_CRITICAL_ISR(&reed.mux) ;
// }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Connects to WiFi  and shows which IP you are connected to afterwards*/
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

/* Goes into deep sleep mode and wakes when the reed switch closes */
void deep_sleep_reed_closed(void) {
    // esp_sleep_enable_ext1_wakeup(0x001000000001, ESP_EXT1_WAKEUP_ANY_HIGH) ;                                   // Configures deep sleep wakeup sources (GPIO)
    esp_sleep_enable_ext0_wakeup(REED_PIN, REED_CLOSED) ;                                   // Configures deep sleep wakeup sources (GPIO)
                                                                                            // then puts the ESP32 into deep sleep mode.
    esp_deep_sleep_start() ;                                                                // Prints wakeup reason when woken up.
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

/* Goes into deep sleep mode and wakes when the reed switch opens */
void deep_sleep_reed_open(void) {
    esp_sleep_enable_ext0_wakeup(REED_PIN, REED_OPEN) ;                                      // Configures deep sleep wakeup sources (GPIO)
                                                                                            // then puts the ESP32 into deep sleep mode.
    esp_deep_sleep_start() ;                                                                // Prints wakeup reason when woken up.
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

/* Goes into deep sleep mode and wakes when the arming button is pressed */
void deep_sleep_button(void) {
    esp_sleep_enable_ext0_wakeup(BUTTON, BUTTON_ON) ;                                      // Configures deep sleep wakeup sources (GPIO)
                                                                                            // then puts the ESP32 into deep sleep mode.
    esp_deep_sleep_start() ;                                                                // Prints wakeup reason when woken up.
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


#ifdef useLightSleep
/* Goes into light sleep mode and wakes when the arming button is pressed */
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

void light_sleep_door_open(void) {                                                                   // Put the device in light sleep mode.
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void light_sleep_door_close(void) {                                                                   // Put the device in light sleep mode.
    esp_sleep_enable_ext0_wakeup(REED_PIN, REED_CLOSED) ;                                      // Configures light sleep wakeup sources (GPIO)
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
#endif  /* useLightSleep */