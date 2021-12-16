#include "main.h"
#include "smtp.h"
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

extern RTC_DATA_ATTR bool volatile doorClosed ;

SMTPSession smtp ;                                                                      // The SMTP Session object used for Email sending

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status){
    /* Print the current status */
    Serial.println( status.info() ) ;

    /* Print the sending result */
    if ( status.success() ) {
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
}   /* End smtpCallback */


/* Send an email using smtp whenever the door closes or opens with the alarm armed */
void send_email(void) {
    My_smtp_t                       mySMTP ;
    ESP_Mail_Session                session ;                                                                   // Declare the session config data
    SMTP_Message                    message ;                                                                   // Declare the message class

    /* Enable the debug via Serial port none debug or 0 basic debug or 1 */
    smtp.debug(1);

    /* Set the callback function to get the sending results */
    smtp.callback(smtpCallback);

    /* Set the session config */
    session.server.host_name    = mySMTP.pSMTP_HOST ;
    session.server.port         = mySMTP.SMTP_PORT ;
    session.login.email         = mySMTP.pAUTHOR_EMAIL ;
    session.login.password      = mySMTP.pAUTHOR_PASSWORD ;
    session.login.user_domain   = "" ;

    /* Set the message headers */
    message.sender.name         = "ESP" ;
    message.sender.email        = mySMTP.pAUTHOR_EMAIL ;

    if ( doorClosed ) {
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
        message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

        /* Set the custom message header */
        message.addHeader("Message-ID: <abcde.fghij@gmail.com>");
    #endif  /* sendRawText */

    /* Connect to server with the session config */
    if ( !smtp.connect(&session) ) {
        return ;
    }

    /* Start sending Email and close the session */
    if ( !MailClient.sendMail(&smtp, &message) ) {
        Serial.println( "Error sending Email, " + smtp.errorReason() ) ;
    }
}