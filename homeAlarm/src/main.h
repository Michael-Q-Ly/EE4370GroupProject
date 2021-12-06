#ifndef MAIN_H_
#define MAIN_H_

#include <Arduino.h>
#include <ESP_Mail_Client.h>

#define BAUD_RATE           115200

#define SMTP_HOST           "smtp.gmail.com"                                                // SMTP host and port
#define SMTP_PORT           465
#define AUTHOR_EMAIL        "YOUR_EMAIL@XXXX.com"                                               // The sign in credentials
#define AUTHOR_PASSWORD     "YOUR_EMAIL_PASS"
#define RECIPIENT_EMAIL     "EE4370Group1@Gmail.com"                                        // Recipient's email

/* -------------------------------------------- Pins -------------------------------------------- */

gpio_num_t const REED_PIN   = GPIO_NUM_12 ;
gpio_num_t const LED_PIN    = GPIO_NUM_16 ;
gpio_num_t const BUTTON     = GPIO_NUM_0 ;

/* -------------------------------------------- Constants -------------------------------------------- */

uint8_t const READ_DELAY        = 100 ;
char const *SSID            = "myNetworkName" ;                                          // Change!
char const *PASS            = "myNetworkPassword" ;                                      // Change!
// uint8_t const   WEB_PORT    = 80 ;
uint8_t const MAX_WIFI_ATTEMPTS = 20 ;

/* -------------------------------------------- Flags -------------------------------------------- */

RTC_DATA_ATTR bool volatile alarmArmed      = false;                                        // Store in RTC memory
RTC_DATA_ATTR bool volatile doorClosed      = true ;
RTC_DATA_ATTR bool volatile wifiEnabled     = false ;

/* -------------------------------------------- Enumerations -------------------------------------------- */

typedef enum {
    REED_CLOSED = LOW,
    REED_OPEN   = HIGH
} Reed_State_t ;


typedef enum {                                                                              // Switches are active LOW
    BUTTON_ON  = LOW,
    BUTTON_OFF = HIGH    
} Button_State_t ;

/* -------------------------------------------- Structures -------------------------------------------- */
Reed_State_t                    reedState ;
esp_sleep_wakeup_cause_t        wakeup_reason ;

SMTPSession smtp;                                                                           // The SMTP Session object used for Email sending
ESP_Mail_Session session ;                                                                  // Declare the session config data
SMTP_Message message ;                                                                      // Declare the message class

/* -------------------------------------------- Global Variables -------------------------------------------- */

/* -------------------------------------------- Function Prototypes -------------------------------------------- */

void init_Wifi(void) ;                                                                      // WiFi

void smtpCallback(SMTP_Status status);                                                      // smtp
void send_email(void) ;

void deep_sleep_reed_closed(void) ;                                                         // Sleep modes
void deep_sleep_reed_open(void) ;
void deep_sleep_button(void) ;

#endif  /* MAIN_H_ */
