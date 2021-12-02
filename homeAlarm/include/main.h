#ifndef MAIN_H_
#define MAIN_H_

#include <Arduino.h>

#define BAUD_RATE           115200

#define SMTP_HOST           "smtp.gmail.com"                                                // SMTP host and port
#define SMTP_PORT           465
#define AUTHOR_EMAIL        "YOUR_EMAIL@XXXX.com"                                               // The sign in credentials
#define AUTHOR_PASSWORD     "YOUR_EMAIL_PASS"
#define RECIPIENT_EMAIL     "EE4370Group1@Gmail.com"                                        // Recipient's email

/* -------------------------------------------- Pins -------------------------------------------- */

gpio_num_t const REED_PIN   = GPIO_NUM_12 ;
gpio_num_t const LED_PIN    = GPIO_NUM_13 ;
gpio_num_t const BUTTON     = GPIO_NUM_0 ;

/* -------------------------------------------- Constants -------------------------------------------- */

uint8_t const READ_DELAY        = 100 ;
char const *SSID            = "myNetworkName" ;          // Change!
char const *PASS            = "myNetworkPassword" ;      // Change!
// uint8_t const   WEB_PORT    = 80 ;
uint8_t const MAX_WIFI_ATTEMPTS = 20 ;

/* -------------------------------------------- Flags -------------------------------------------- */

RTC_DATA_ATTR bool volatile alarmArmed      = false;            // Store in RTC memory
RTC_DATA_ATTR bool volatile doorClosed      = true ;
RTC_DATA_ATTR bool volatile wifiEnabled     = false ;

/* -------------------------------------------- Enumerations -------------------------------------------- */

typedef enum {
    REED_CLOSED = LOW,
    REED_OPEN   = HIGH
} Reed_State_t ;

typedef enum {                                                                              // Double check if active high or low
    LED_ON      = HIGH ,
    LED_OFF     = LOW
} LED_State_t ;

typedef enum {                                                                              // Switches are active LOW
    BUTTON_ON  = LOW,
    BUTTON_OFF = HIGH    
} Button_State_t ;

/* -------------------------------------------- Structures -------------------------------------------- */

// typedef struct {
//     uint16_t const              DEBOUNCE    = 500 ;
//     uint8_t volatile            count ;
//     portMUX_TYPE                mux         = portMUX_INITIALIZER_UNLOCKED ;
//     uint32_t volatile           interruptCounter ;
//     uint32_t                    totalInterruptCounter ;
// } Button_t ;

// typedef struct {
//     uint16_t const              DEBOUNCE    = 500 ;
//     uint8_t volatile            count ;
//     portMUX_TYPE                mux         = portMUX_INITIALIZER_UNLOCKED ;
//     uint32_t volatile           interruptCounter ;
//     uint32_t                    totalInterruptCounter ;
// } Reed_t ;

/* -------------------------------------------- Typedef Variables -------------------------------------------- */
// Button_t                        button ;
// Reed_t                          reed ;
LED_State_t                     ledState ;
Reed_State_t                    reedState ;
esp_sleep_wakeup_cause_t        wakeup_reason ;

/* -------------------------------------------- Global Variables -------------------------------------------- */
portMUX_TYPE                buttonMUX                     = portMUX_INITIALIZER_UNLOCKED ;
uint32_t volatile           buttonInterruptCounter ;
RTC_DATA_ATTR uint32_t      buttonTotalInterruptCounter ;

/* -------------------------------------------- Function Prototypes -------------------------------------------- */

void init_Wifi(void) ;                                                  // WiFi

// void checkReedState(void) ;                                          // Reed Switch

void IRAM_ATTR ISR_buttonPressed(void) ;                                // Interrupt handlers
// void IRAM_ATTR ISR_reedOpen(void) ;

void deep_sleep_reed_closed(void) ;                                     // Sleep modes
void deep_sleep_reed_open(void) ;
void deep_sleep_button(void) ;
#ifdef useLightsleep
    void light_sleep_button(void) ;
    void light_sleep_door_open(void) ;
    void light_sleep_door_closed(void) ;
#endif  /* useLightSleep */


// void checkButtonPress(void) ;

#endif  /* MAIN_H_ */
