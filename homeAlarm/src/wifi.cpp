#include "main.h"
#include "my_wifi.h"
#include <WiFi.h>
#include <stdint.h>

void initWifi(void) {
    uint8_t tries;
    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA) ;
    WiFi.disconnect() ;
    delay(100) ;

    WiFi.begin(SSID, PASS) ;
    Serial.printf("Connection to WiFi...\n") ;
    tries = 0 ;
    while ( (WiFi.status() != WL_CONNECTED) || (++tries < 20) ) {
        Serial.printf(".") ;
        delay(1000) ;
    }
    Serial.printf("You are now connected to ") ;
    Serial.println( WiFi.localIP() ) ;
}