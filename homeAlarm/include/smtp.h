#ifndef SMTP_H_
#define SMTP_H_

typedef struct {
    char const  *pSMTP_HOST           = "smtp.gmail.com" ;
    uint16_t     SMTP_PORT            = 465 ;
    char const  *pAUTHOR_EMAIL        = "YOUR_EMAIL@XXXX.com" ;                                         // The sign in credentials
    char const  *pAUTHOR_PASSWORD     = "YOUR_EMAIL_PASSWORD" ;
    char const  *pRECIPIENT_EMAIL     = "YOUR_RECPIENT_EMAIL@XXXX.COM" ;                                      // Recipient's email
} My_smtp_t ;

#endif  /* SMTP_H_ */
