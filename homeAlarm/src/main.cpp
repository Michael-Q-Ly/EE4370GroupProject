#include "main.h"
// #include "reedSwitch.h"
// #include "button_arm.h"

#include <Wifi.h>
// #include <ESP_Mail_Client.h>
#include <time.h>
#include <stdint.h>

#define mS_2_S_FACTOR       1000
#define SET_UP_TIME         3

void setup() {
    Serial.begin(BAUD_RATE) ;

    pinMode(BUTTON_ARM,     INPUT_PULLUP) ;
    pinMode(BUTTON_DISARM,  INPUT_PULLUP) ;
    pinMode(REED_PIN,       INPUT_PULLUP) ;
    pinMode(LED_PIN,        OUTPUT) ;

    Serial.printf("Door state is %d\n", doorClosed) ;
    Serial.printf("Alarm state is %s\n", alarmArmed ? "ON" : "OFF") ;
    delay(100) ;

    /* Maybe we can make this an FSM instead? */
    digitalWrite( LED_PIN, LED_ON ) ;
    uint32_t current_millis ;
    bool disarmButtonPressed ;
    bool armButtonPressed ;
    current_millis = millis() ;

    // Have a delay time to set up the alarm and to disarm it.
    Serial.printf("Setting up... Press disarm button to reset.\n") ;
    while ( millis() - current_millis <= SET_UP_TIME * mS_2_S_FACTOR ) {
        disarmButtonPressed = (bool)digitalRead( BUTTON_DISARM ) ;
        armButtonPressed = (bool)digitalRead( BUTTON_ARM ) ;
        if ( disarmButtonPressed != BUTTON_OFF ) {
            alarmArmed = false ;
            wifiEnabled = false ;
            Serial.printf("Going to deep sleep mode...\n") ;
            Serial.printf("Alarm state is %s\n", alarmArmed ? "ON" : "OFF") ;
            delay(200) ;
            goto disarmed ;
        }
        if ( armButtonPressed != BUTTON_OFF ) {
            goto armed ;
        }
    }

    if (wifiEnabled) {
        Serial.printf("Connecting to WiFi...\n") ;
        init_Wifi() ;

        send_email() ;

        wifiEnabled = false ;
    }
    
    disarmed:
    if ( ! (alarmArmed) )  {
        digitalWrite( LED_PIN, LED_OFF ) ;
        Serial.println("Going to deep sleep mode until button press...") ;
        delay(100) ;
        alarmArmed = true ;
        deep_sleep_button() ;
    }
    else {
        goto armed ;
    }
    armed:
        if (doorClosed) {

            Serial.println("Alarm is armed. Going to deep sleep mode. Wake by opening door.\n") ;
            delay(100) ;
            doorClosed  = false ;
            wifiEnabled = true ;
            deep_sleep_reed_open() ;
        }
        else {
            Serial.println("Alarm is armed. Going to deep sleep mode. Wake by closing door.\n") ;
            delay(100) ;
            doorClosed  = true ;
            wifiEnabled = true ;
            deep_sleep_reed_closed() ;
        }

}   /* End setup() */

void loop() {
    // Nothing goes here. The board is reset every time we go to deep sleep.
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* FUNCTION PROTOTYPES */

/* Connects to WiFi  and shows which IP you are connected to afterwards*/
void init_Wifi(void) {
    uint8_t tries;
    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA) ;
    WiFi.disconnect() ;
    delay(100) ;

    WiFi.begin(SSID, PASS) ;
    tries = 0 ;
    while ( (++tries < MAX_WIFI_ATTEMPTS) || (WiFi.status() != WL_CONNECTED) ) {
        Serial.printf(".") ;
        delay(100) ;
    }
    Serial.printf("You are now connected to ") ;
    Serial.println( WiFi.localIP() ) ;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Goes into deep sleep mode and wakes when the reed switch closes */
void deep_sleep_reed_closed(void) {
    // esp_sleep_enable_ext1_wakeup(0x001000000001, ESP_EXT1_WAKEUP_ANY_HIGH) ;
    esp_sleep_enable_ext0_wakeup(REED_PIN, REED_CLOSED) ;                                   // Configures deep sleep wakeup sources (GPIO)
    esp_sleep_enable_ext1_wakeup( 0x01, ESP_EXT1_WAKEUP_ALL_LOW ) ;                                   // Configures deep sleep wakeup sources (GPIO)
                                                                                            // then puts the ESP32 into deep sleep mode.
    esp_deep_sleep_start() ;                                                                // Prints wakeup reason when woken up.

    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();
    switch(wakeup_reason)
    {
        case ESP_SLEEP_WAKEUP_EXT0      : 
            Serial.println("\nWakeup caused by external signal using RTC_IO") ;
            alarmArmed = !alarmArmed ;
            break ;
        case ESP_SLEEP_WAKEUP_EXT1      :
        alarmArmed = false ;
        Serial.println("\nWakeup caused by external signal using RTC_CNTL") ;
        delay(100) ;
        break ;
        case ESP_SLEEP_WAKEUP_TIMER     : Serial.println("\nWakeup caused by timer") ;                                    break ;
        case ESP_SLEEP_WAKEUP_TOUCHPAD  : Serial.println("\nWakeup caused by touchpad") ;                                 break ;
        case ESP_SLEEP_WAKEUP_ULP       : Serial.println("\nWakeup caused by ULP program") ;                              break ;
        default                         : Serial.printf("\nWakeup was not caused by deep sleep: %d\n", wakeup_reason) ;   break ;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Goes into deep sleep mode and wakes when the reed switch opens */
void deep_sleep_reed_open(void) {
    esp_sleep_enable_ext0_wakeup(REED_PIN, REED_OPEN) ;                                     // Configures deep sleep wakeup sources (GPIO)
    esp_sleep_enable_ext1_wakeup( 0x01, ESP_EXT1_WAKEUP_ALL_LOW ) ;                                   // Configures deep sleep wakeup sources (GPIO)
                                                                                            // then puts the ESP32 into deep sleep mode.
    esp_deep_sleep_start() ;                                                                // Prints wakeup reason when woken up.

    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();
    switch(wakeup_reason)
    {
        case ESP_SLEEP_WAKEUP_EXT0      : 
            Serial.println("\nWakeup caused by external signal using RTC_IO") ;
            alarmArmed = !alarmArmed ;
            break ;
        case ESP_SLEEP_WAKEUP_EXT1      :
        alarmArmed = false ;
        Serial.println("\nWakeup caused by external signal using RTC_CNTL") ;
        delay(100) ;
        break ;
        case ESP_SLEEP_WAKEUP_TIMER     : Serial.println("\nWakeup caused by timer") ;                                    break ;
        case ESP_SLEEP_WAKEUP_TOUCHPAD  : Serial.println("\nWakeup caused by touchpad") ;                                 break ;
        case ESP_SLEEP_WAKEUP_ULP       : Serial.println("\nWakeup caused by ULP program") ;                              break ;
        default                         : Serial.printf("\nWakeup was not caused by deep sleep: %d\n", wakeup_reason) ;   break ;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Goes into deep sleep mode and wakes when the arming button is pressed */
void deep_sleep_button(void) {
    esp_sleep_enable_ext0_wakeup(BUTTON_ARM, BUTTON_ON) ;                                       // Configures deep sleep wakeup sources (GPIO)
                                                                                            // then puts the ESP32 into deep sleep mode.
    esp_deep_sleep_start() ;                                                                // Prints wakeup reason when woken up.
    wakeup_reason = esp_sleep_get_wakeup_cause() ;
    switch(wakeup_reason)
    {
        case ESP_SLEEP_WAKEUP_EXT0      : Serial.println("Wakeup caused by external signal using RTC_IO") ;    alarmArmed = !alarmArmed ;         break ;
        case ESP_SLEEP_WAKEUP_EXT1      : Serial.println("Wakeup caused by external signal using RTC_CNTL") ;           break ;
        case ESP_SLEEP_WAKEUP_TIMER     : Serial.println("Wakeup caused by timer") ;                                    break ;
        case ESP_SLEEP_WAKEUP_TOUCHPAD  : Serial.println("Wakeup caused by touchpad") ;                                 break ;
        case ESP_SLEEP_WAKEUP_ULP       : Serial.println("Wakeup caused by ULP program") ;                              break ;
        default                         : Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason) ;   break ;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status){
    /* Print the current status */
    Serial.println( status.info() ) ;

    /* Print the sending result */
    if (status.success()) {
        Serial.println("----------------");
        ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount()) ;
        ESP_MAIL_PRINTF("Message sent failled: %d\n", status.failedCount()) ;
        Serial.println("----------------\n");
        struct tm dt;

        for (size_t i = 0; i < smtp.sendingResult.size(); i++) {
            /* Get the result item */
            SMTP_Result result = smtp.sendingResult.getItem(i) ;
            time_t ts = (time_t)(result.timestamp) ;
            localtime_r(&ts, &dt) ;

            ESP_MAIL_PRINTF("Message No: %d\n", i + 1) ;
            ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed") ;
            ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec) ;
            ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients) ;
            ESP_MAIL_PRINTF("Subject: %s\n", result.subject) ;
        }
        Serial.println("----------------\n");
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
MIT License

Copyright (c) 2021 mobizt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
Example adapted from: https://github.com/mobizt/ESP-Mail-Client
*/

/* Send an email using smtp whenever the door closes or opens with the alarm armed */
void send_email(void) {
    /* Enable the debug via Serial port none debug or 0 basic debug or 1 */
    smtp.debug(1);

    /* Set the callback function to get the sending results */
    smtp.callback(smtpCallback);

    // /* Declare the session config data */
    // ESP_Mail_Session session;

    /* Set the session config */
    session.server.host_name    = mySMTP.pSMTP_HOST ;
    session.server.port         = mySMTP.SMTP_PORT ;
    session.login.email         = mySMTP.pAUTHOR_EMAIL ;
    session.login.password      = mySMTP.pAUTHOR_PASSWORD ;
    session.login.user_domain   = "" ;

    // /* Declare the message class */
    // SMTP_Message message;

    /* Set the message headers */
    message.sender.name         = "ESP" ;
    message.sender.email        = mySMTP.pAUTHOR_EMAIL ;

    // message.subject             = "ESP Test Email" ;
    if (doorClosed) {
        message.subject         = "Door is closed" ;
    }
    else {
        message.subject         = "Door is Open" ;
    }

    message.addRecipient("Embedded Student", mySMTP.pRECIPIENT_EMAIL) ;

    /* Send HTML message */
    String htmlMsg              =   "<div style=\"color:#2f4468;\">"
                                        "<h1>DOOR STATUS</h1>"
                                            "<p>- Sent from ESP board</p>"
                                    "</div>" ;
    message.html.content        = htmlMsg.c_str() ;
    message.html.content        = htmlMsg.c_str() ;
    message.text.charSet        = "us-ascii" ;
    message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit ;

    #ifdef sendRawText
        //Send raw text message
        String textMsg                  = "Hello World! - Sent from ESP board";
        message.text.content            = textMsg.c_str();
        message.text.charSet            = "us-ascii";
        message.text.transfer_encoding  = Content_Transfer_Encoding::enc_7bit;

        message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
        message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;*/

        /* Set the custom message header */
        message.addHeader("Message-ID: <abcde.fghij@gmail.com>");
    #endif  /* sendRawText */

    /* Connect to server with the session config */
    if (!smtp.connect(&session)) {
        return;
    }

    /* Start sending Email and close the session */
    if (!MailClient.sendMail(&smtp, &message)) {
        Serial.println("Error sending Email, " + smtp.errorReason()) ;
    }
}