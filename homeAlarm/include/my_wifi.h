#ifndef MY_WIFI_H_
#define MY_WIFI_H_

#include <stdint.h>
typedef struct my_wifi {
    char const *SSID                    = "myNetworkName" ;
    char const *PASS                    = "myNetworkPassword" ;
    uint8_t const MAX_WIFI_ATTEMPTS     = 20 ;
} My_Wifi_t ;

#endif /* MY_WIFI_H_ */
