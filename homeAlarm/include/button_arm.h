#ifndef BUTTON_ARM_H_
#define BUTTON_ARM_H

#include <Arduino.h>
#include <stdint.h>

gpio_num_t const BUTTON = GPIO_NUM_1 ;

typedef enum {                                                                              // Switches are active LOW
    BUTTON_ON  = LOW,
    BUTTON_OFF = HIGH    
} Button_State_t ;

typedef struct {
    uint8_t volatile            count ;
    portMUX_TYPE                mux = portMUX_INITIALIZER_UNLOCKED ;
    uint32_t volatile           interruptCounter ;
    uint32_t                    totalInterruptCounter ;
} Button_t ;

/* Typedef Variables */
// Button_t                        button ;

#endif  /* BUTTON_ARM_H_ */