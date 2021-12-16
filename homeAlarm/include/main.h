#ifndef MAIN_H_
#define MAIN_H_

#include "my_wifi.h"                     // If you have your own wifi header file
#include "smtpcredentials.h"            // If you have your own smtp header file

#include <ESP_Mail_Client.h>
#include <Arduino.h>
#include <stdint.h>

#define BAUD_RATE                           115200

#define EXAMPLE_SMTP_HOST                   "smtp.gmail.com"                                                // SMTP host and port
#define EXAMPLE_SMTP_PORT                   465
#define EXAMPLE_AUTHOR_EMAIL                "YOUR_EMAIL@XXXX.com"                                           // The sign in credentials
#define EXAMPLE_AUTHOR_PASSWORD             "YOUR_EMAIL_PASS"
#define EXAMPLE_RECIPIENT_EMAIL             "EE4370Group1@Gmail.com"                                        // Recipient's email

/* -------------------------------------------- Pins -------------------------------------------- */

gpio_num_t const REED_PIN                   = GPIO_NUM_12 ;
gpio_num_t const LED_PIN                    = GPIO_NUM_13 ;
gpio_num_t const BUTTON_ARM                 = GPIO_NUM_14 ;
gpio_num_t const BUTTON_DISARM              = GPIO_NUM_15 ;

/* -------------------------------------------- Constants -------------------------------------------- */

uint8_t const READ_DELAY                    = 100 ;

char *EXAMPLE_SSID                          = "myNetworkName" ;                                          // Change name by omitting 'EXAMPLE_'!
char *EXAMPLE_PASS                          = "myNetworkPassword" ;                                      // Change name by omitting 'EXAMPLE_'!
uint8_t const MAX_WIFI_ATTEMPTS             = 20 ;

/* -------------------------------------------- Flags -------------------------------------------- */

RTC_DATA_ATTR bool volatile alarmArmed      = false ;                                        // Store in RTC memory
RTC_DATA_ATTR bool volatile doorClosed      = true ;
RTC_DATA_ATTR bool volatile wifiEnabled     = false ;

/* -------------------------------------------- Enumerations -------------------------------------------- */

typedef enum {
    REED_CLOSED = LOW,
    REED_OPEN   = HIGH
} Reed_State_t ;

typedef enum {                                                                           // Double check if LED is active high or low
    LED_ON      = HIGH ,
    LED_OFF     = LOW
} LED_State_t ;

typedef enum {                                                                              // Switches are active LOW
    BUTTON_ON  = LOW,
    BUTTON_OFF = HIGH    
} Button_State_t ;

/* -------------------------------------------- Structures -------------------------------------------- */

typedef struct {
    uint16_t const              DEBOUNCE    = 500 ;
    uint8_t volatile            count ;
    portMUX_TYPE                mux         = portMUX_INITIALIZER_UNLOCKED ;
    uint32_t volatile           interruptCounter ;
    uint32_t                    totalInterruptCounter ;
} Button_t ;

/* -------------------------------------------- Typedef Variables -------------------------------------------- */
RTC_DATA_ATTR Button_t          button ;
Button_State_t                  buttonPressed = BUTTON_OFF ;
// Reed_t                          reed ;
// LED_State_t                     ledState ;
Reed_State_t                    reedState ;
esp_sleep_wakeup_cause_t        wakeup_reason ;

My_smtp_t                       mySMTP ;
SMTPSession                     smtp ;                                                                           // The SMTP Session object used for Email sending
ESP_Mail_Session                session ;                                                                  // Declare the session config data
SMTP_Message                    message ;                                                                      // Declare the message class

/* -------------------------------------------- Global Variables -------------------------------------------- */

/* -------------------------------------------- Function Prototypes -------------------------------------------- */

extern void init_Wifi(void) ;                                                                      // WiFi

extern void smtpCallback(SMTP_Status status);                                                      // smtp
extern void send_email(void) ;

extern void deep_sleep_door_closed(void) ;                                                         // Sleep modes
extern void deep_sleep_door_open(void) ;
extern void deep_sleep_button(void) ;

#endif  /* MAIN_H_ */
