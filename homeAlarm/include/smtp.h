#ifndef SMTP_H_
#define SMTP_H_

typedef struct {
    char const  *pSMTP_HOST           = "smtp.gmail.com" ;
    uint16_t     SMTP_PORT            = 465 ;
    char const  *pAUTHOR_EMAIL        = "YOUR_EMAIL@XXXX.com" ;                                         // The sign in credentials
    char const  *pAUTHOR_PASSWORD     = "EmbeddedSystems4370#" ;
    char const  *pRECIPIENT_EMAIL     = "EE4370Group1@Gmail.com" ;                                      // Recipient's email
} My_smtp_t ;

#endif  /* SMTP_H_ */