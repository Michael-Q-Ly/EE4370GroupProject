// #include "main.h"
// #include "reedSwitch.h"

// /*
//  * Reads the digital pin of the reed switch and turns an LED on if it is open
//  */
// void checkReedState() {
//     Reed_State_t    reedState ;
//     LED_State_t     ledState ;

//     reedState = (Reed_State_t)( digitalRead(REED_PIN) ) ;

//     if (reedState != REED_CLOSED) {                                             // LED turns on if reed isn't closed
//         ledState = LED_ON ;
//         digitalWrite(LED_PIN, ledState) ;
//     }
//     else {
//         ledState = LED_OFF ;
//         digitalWrite(LED_PIN, ledState) ;
//     }

//     delay(READ_DELAY) ;
// }