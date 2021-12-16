#ifndef MAIN_H_
#define MAIN_H_

#include <Arduino.h>
#include <ESP_Mail_Client.h>
#include <stdint.h>

#define BAUD_RATE                           115200
#define mS_2_S_FACTOR                       1000
#define SET_UP_TIME                         3

/* -------------------------------------------- Pins -------------------------------------------- */

gpio_num_t const REED_PIN                   = GPIO_NUM_12 ;
gpio_num_t const LED_PIN                    = GPIO_NUM_13 ;
gpio_num_t const ARM_PIN                    = GPIO_NUM_14 ;
gpio_num_t const DISARM_PIN                 = GPIO_NUM_15 ;
/* -------------------------------------------- Enumerations -------------------------------------------- */

typedef enum {
    REED_CLOSED = LOW,
    REED_OPEN   = HIGH
} Reed_State_t ;

typedef enum {
    LED_ON      = HIGH ,
    LED_OFF     = LOW
} LED_State_t ;

typedef enum {
    BUTTON_ON  = LOW,
    BUTTON_OFF = HIGH    
} Button_State_t ;

/* -------------------------------------------- Function Prototypes -------------------------------------------- */

extern void init_Wifi(void) ;                                                                      // WiFi
extern void smtpCallback(SMTP_Status status);                                                      // smtp
extern void send_email(void) ;

#endif  /* MAIN_H_ */
