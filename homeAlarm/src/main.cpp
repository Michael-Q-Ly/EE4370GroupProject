#include "main.h"

/* Data stored in RTC memory */
RTC_DATA_ATTR bool volatile alarmArmed      = false ;                                        // Store in RTC memory
RTC_DATA_ATTR bool volatile doorClosed      = true ;
RTC_DATA_ATTR bool volatile wifiEnabled     = false ;

LED_State_t                 ledState ;
esp_sleep_wakeup_cause_t    wakeup_reason ;
uint32_t                    current_millis ;

/* Function Prototypes */
static void deep_sleep_door_closed(void) ;                                                         // Sleep modes
static void deep_sleep_door_open(void) ;
static void deep_sleep_button(void) ;

void setup() {
    Serial.begin( BAUD_RATE ) ;

    pinMode( BUTTON_ARM,     INPUT_PULLUP ) ;
    pinMode( BUTTON_DISARM,  INPUT_PULLUP ) ;
    pinMode( REED_PIN,       INPUT_PULLUP ) ;
    pinMode( LED_PIN,        OUTPUT ) ;
    

    bool disarmButtonPressed ;
    bool armButtonPressed ;
    
    // Briefly turn an LED on during set up time
    ledState = LED_ON ;
    digitalWrite( LED_PIN, ledState ) ;

    Serial.printf( "Door state is %s\n", doorClosed ? "CLOSED" : "OPEN" ) ;
    Serial.printf( "Alarm state is %s\n", alarmArmed ? "ON" : "OFF" ) ;
    delay( 100 ) ;
    
    // Get the current time
    current_millis = millis() ;

    // Brief set up period in which the user may disarm the alarm. Happens every wake up call from deep sleep.
    Serial.printf("Setting up... Press disarm button to reset.\n") ;
    while ( millis() - current_millis <= SET_UP_TIME * mS_2_S_FACTOR ) {
        
        // Get the state of each button
        disarmButtonPressed = ( bool ) digitalRead( BUTTON_DISARM ) ;
        armButtonPressed    = ( bool ) digitalRead( BUTTON_ARM ) ;

        // Go to an armed or disarmed label depending on which button the user presses
        if ( disarmButtonPressed != BUTTON_OFF ) {
            alarmArmed  = false ;
            wifiEnabled = false ;
            Serial.printf("Going to deep sleep mode...\n") ;
            Serial.printf( "Alarm state is %s\n", alarmArmed ? "ON" : "OFF" ) ;
            delay( 200 ) ;
            goto disarmed ;
        }
        
        if ( armButtonPressed != BUTTON_OFF ) {
            goto armed ;
        }
    }
    
    // When the device wakes up, check if wifi is enabled, then connect and send an email of door state
    if ( wifiEnabled ) {
        Serial.printf( "Connecting to WiFi...\n" ) ;
        init_Wifi() ;
        send_email() ;
        wifiEnabled = false ;
    }
    
    disarmed:
    if ( ! ( alarmArmed ) )  {
        ledState = LED_OFF ;
        digitalWrite( LED_PIN, ledState ) ;
        Serial.println( "Going to deep sleep mode until button press..." ) ;
        delay( 100 ) ;
        alarmArmed = true ;
        deep_sleep_button() ;
    }
    else {
        goto armed ;
    }
    armed:
        if ( doorClosed ) {
            Serial.println( "Alarm is armed. Going to deep sleep mode. Wake by opening door.\n" ) ;
            delay( 100 ) ;
            doorClosed  = false ;
            wifiEnabled = true ;
            deep_sleep_door_open() ;
        }
        else {
            Serial.println( "Alarm is armed. Going to deep sleep mode. Wake by closing door.\n" ) ;
            delay( 100 ) ;
            doorClosed  = true ;
            wifiEnabled = true ;
            deep_sleep_door_closed() ;
        }

}   /* End setup() */

void loop() {
    // Nothing goes here. The board is reset every time we go to deep sleep.
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* FUNCTION PROTOTYPES */

/* Goes into deep sleep mode and wakes when the reed switch closes */
static void deep_sleep_door_closed(void) {
    esp_sleep_enable_ext0_wakeup(REED_PIN, REED_CLOSED) ;
    esp_deep_sleep_start() ;

    wakeup_reason = esp_sleep_get_wakeup_cause();
    switch(wakeup_reason)
    {
        case ESP_SLEEP_WAKEUP_EXT0      : Serial.println("\nWakeup caused by external signal using RTC_IO") ;               break ;
        case ESP_SLEEP_WAKEUP_EXT1      : Serial.println("\nWakeup caused by external signal using RTC_CNTL") ;             break ;
        case ESP_SLEEP_WAKEUP_TIMER     : Serial.println("\nWakeup caused by timer") ;                                      break ;
        case ESP_SLEEP_WAKEUP_TOUCHPAD  : Serial.println("\nWakeup caused by touchpad") ;                                   break ;
        case ESP_SLEEP_WAKEUP_ULP       : Serial.println("\nWakeup caused by ULP program") ;                                break ;
        default                         : Serial.printf("\nWakeup was not caused by deep sleep: %d\n", wakeup_reason) ;     break ;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Goes into deep sleep mode and wakes when the reed switch opens */
static void deep_sleep_door_open(void) {
    esp_sleep_enable_ext0_wakeup(REED_PIN, REED_OPEN) ;
    esp_deep_sleep_start() ;

    wakeup_reason = esp_sleep_get_wakeup_cause();
    switch(wakeup_reason)
    {
        case ESP_SLEEP_WAKEUP_EXT0      : Serial.println("\nWakeup caused by external signal using RTC_IO") ;               break ;
        case ESP_SLEEP_WAKEUP_EXT1      : Serial.println("\nWakeup caused by external signal using RTC_CNTL") ;             break ;
        case ESP_SLEEP_WAKEUP_TIMER     : Serial.println("\nWakeup caused by timer") ;                                      break ;
        case ESP_SLEEP_WAKEUP_TOUCHPAD  : Serial.println("\nWakeup caused by touchpad") ;                                   break ;
        case ESP_SLEEP_WAKEUP_ULP       : Serial.println("\nWakeup caused by ULP program") ;                                break ;
        default                         : Serial.printf("\nWakeup was not caused by deep sleep: %d\n", wakeup_reason) ;     break ;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Goes into deep sleep mode and wakes when the arming button is pressed */
static void deep_sleep_button(void) {
    esp_sleep_enable_ext0_wakeup(BUTTON_ARM, BUTTON_ON) ;
    esp_deep_sleep_start() ;

    wakeup_reason = esp_sleep_get_wakeup_cause() ;
    switch(wakeup_reason)
    {
        case ESP_SLEEP_WAKEUP_EXT0      : Serial.println("Wakeup caused by external signal using RTC_IO") ;                 break ;
        case ESP_SLEEP_WAKEUP_EXT1      : Serial.println("Wakeup caused by external signal using RTC_CNTL") ;               break ;
        case ESP_SLEEP_WAKEUP_TIMER     : Serial.println("Wakeup caused by timer") ;                                        break ;
        case ESP_SLEEP_WAKEUP_TOUCHPAD  : Serial.println("Wakeup caused by touchpad") ;                                     break ;
        case ESP_SLEEP_WAKEUP_ULP       : Serial.println("Wakeup caused by ULP program") ;                                  break ;
        default                         : Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason) ;       break ;
    }
}