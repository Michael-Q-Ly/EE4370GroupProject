#ifndef MAIN_H_
#define MAIN_H_

#include <Arduino.h>

#define BAUD_RATE       115200

/* Pins */
gpio_num_t const REED_PIN   = GPIO_NUM_12 ;
gpio_num_t const LED_PIN    = GPIO_NUM_13 ;
gpio_num_t const BUTTON     = GPIO_NUM_0 ;

bool wifi_enabled ;


/* Constants */

int const READ_DELAY        = 100 ;
char const *SSID            = "Misnomer" ;
char const *PASS            = "3mb3d3dSystems!" ;
// uint8_t const   WEB_PORT    = 80 ;

/* Enumerations */

typedef enum {
    REED_CLOSED = LOW,
    REED_OPEN   = HIGH
} Reed_State_t ;

typedef enum {
    LED_ON      = HIGH ,
    LED_OFF     = LOW
} LED_State_t ;

typedef enum {                                                                              // Switches are active LOW
    BUTTON_ON  = LOW,
    BUTTON_OFF = HIGH    
} Button_State_t ;

/* Structures */

typedef struct {
    uint16_t const              DEBOUNCE    = 500 ;
    uint8_t volatile            count ;
    portMUX_TYPE                mux         = portMUX_INITIALIZER_UNLOCKED ;
    uint32_t volatile           interruptCounter ;
    uint32_t                    totalInterruptCounter ;
} Button_t ;

/* Typedef Variables */
Button_t                        button ;





// WiFi
void initWifi() ;
// Reed Switch
void checkReedState() ;
// Button
void IRAM_ATTR ISR_buttonPressed(void) ;
void checkButtonPress(void) ;

#endif  /* MAIN_H_ */
