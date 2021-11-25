#ifndef MAIN_H_
#define MAIN_H_

#include <Arduino.h>

#define BAUD_RATE       115200

extern void initWifi() ;
extern void checkReedState() ;
extern void IRAM_ATTR ISR_buttonPressed(void) ;

#endif  /* MAIN_H_ */