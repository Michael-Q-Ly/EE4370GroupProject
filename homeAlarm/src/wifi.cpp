#include "my_wifi.h"
#include <WiFi.h>

/* Connects to WiFi  and shows which IP you are connected to afterwards*/
void init_Wifi(void) {
    uint8_t tries;
    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA) ;
    WiFi.disconnect() ;
    delay( 100 ) ;

    WiFi.begin(SSID, PASS) ;
    tries = 0 ;
    while ( (++tries < MAX_WIFI_ATTEMPTS) || (WiFi.status() != WL_CONNECTED) ) {
        Serial.printf(".") ;
        delay( 100 ) ;
    }
    Serial.printf("You are now connected to ") ;
    Serial.println( WiFi.localIP() ) ;
}