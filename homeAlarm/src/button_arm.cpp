#include "main.h"
#include "button_arm.h"

#include <Arduino.h>

Button_t                        button ;

/* Arming button interrupt service routine. Increases an interrupt counter. */
void IRAM_ATTR ISR_buttonPressed(void) {
    portENTER_CRITICAL_ISR(&button.mux) ;
    button.interruptCounter++ ;
    portEXIT_CRITICAL_ISR(&button.mux) ;
}

/* Checks for a button press by incrementing a counter then decrementing it back
 * after using a spinlock. Keeps track of total presses with second variable
 */
void checkButtonPress(void) {
    bool wifi_switch ;
    // If the arm button is pressed, then begin checking the reed's state.
    if (button.interruptCounter > 0) {
        portENTER_CRITICAL(&button.mux) ;
        button.interruptCounter-- ;
        portEXIT_CRITICAL(&button.mux) ;

         // Interrupt handling code
        button.totalInterruptCounter++ ;
        Serial.printf("A button interrupt has occurred. Total Number: %u\n", button.totalInterruptCounter) ;
    }

    if (button.totalInterruptCounter % 2) {                         // Every 2 presses turns off wifi flag
        wifi_switch = true ;
    }
    else {
        wifi_switch = false ;
    }

    if (wifi_switch) {
        initWifi() ;
        wifi_switch = false ;
        wifi_enabled = true ;
    }

}

