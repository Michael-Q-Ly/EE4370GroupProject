#ifndef MAIN_H_
#define MAIN_H_

#include <Arduino.h>

#define BAUD_RATE       115200

/* -------------------------------------------- Pins -------------------------------------------- */

gpio_num_t const REED_PIN   = GPIO_NUM_12 ;
gpio_num_t const LED_PIN    = GPIO_NUM_13 ;
gpio_num_t const BUTTON     = GPIO_NUM_0 ;

bool wifi_enabled ;


/* -------------------------------------------- Constants -------------------------------------------- */

uint8_t const READ_DELAY        = 100 ;
char const *SSID                = "HuisATT 2.4GHz" ;
char const *PASS                = "coffeesluts" ;
// char const *SSID            = "myNetworkName" ;
// char const *PASS            = "myNetworkPassword" ;
// uint8_t const   WEB_PORT    = 80 ;
uint8_t const MAX_WIFI_ATTEMPTS = 20 ;

/* Enumerations */

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



/* -------------------------------------------- Function Prototypes -------------------------------------------- */
// WiFi
void init_Wifi(void) ;
// Reed Switch
// void checkReedState(void) ;
// Interrupt handlers
// void IRAM_ATTR ISR_buttonPressed(void) ;
// void IRAM_ATTR ISR_reedOpen(void) ;
// Sleep modes
void light_sleep_button(void) ;
void light_sleep_switch(void) ;
void deep_sleep(void) ;

// void checkButtonPress(void) ;

#endif  /* MAIN_H_ */
