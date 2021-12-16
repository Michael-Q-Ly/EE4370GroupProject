#include "main.h"
#include "my_wifi.h"
#include <WiFi.h>

/* Connects to WiFi  and shows which IP you are connected to afterwards*/
void init_Wifi(void) {
    My_Wifi_t wifi ;
    uint8_t tries;
    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA) ;
    WiFi.disconnect() ;
    delay( 100 ) ;

    WiFi.begin(wifi.SSID, wifi.PASS) ;
    tries = 0 ;
    while ( (++tries < wifi.MAX_WIFI_ATTEMPTS) || (WiFi.status() != WL_CONNECTED) ) {
        Serial.printf(".") ;
        delay( 100 ) ;
    }
    Serial.printf("You are now connected to ") ;
    Serial.println( WiFi.localIP() ) ;
}