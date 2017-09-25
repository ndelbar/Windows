import smtplib
import time
import imaplib
import email
import localIni
# -------------------------------------------------
#
# Utility to read email from Gmail Using Python
#
# ------------------------------------------------

strKeySubject = "SMSHELPME"
strKeyUseStocks = "USESTOCKS:"

def read_email_from_gmail():
    try:
        mail = imaplib.IMAP4_SSL(SMTP_SERVER)
        mail.login(FROM_EMAIL,FROM_PWD)
        mail.select('inbox')

        type, data = mail.search(None, 'ALL')
        mail_ids = data[0]

        id_list = mail_ids.split()   
        first_email_id = int(id_list[0])
        latest_email_id = int(id_list[-1])
        print(first_email_id)
        print(latest_email_id)
        for i in range(latest_email_id,first_email_id - 1, -1):
            typ, data = mail.fetch(str(i), '(RFC822)' )
            for response_part in data:
                if isinstance(response_part, tuple):
                    msg = str(email.message_from_string(str(response_part[1]))).upper().replace(' ','')
                    if msg.find(strKeySubject) != -1:
                        print(msg)
                        iSubjectStart = msg.find(strKeyUseStocks) + len(strKeyUseStocks)
                        iSubjectStop = msg.find('\\',iSubjectStart)
                        strCommand = msg[iSubjectStart:iSubjectStop]
                        print(iSubjectStart)
                        print(iSubjectStop)
                        print(strCommand)
                        Stocks = strCommand.split(',')
                        print(Stocks)
                        localIni.AddList('ndelbar',strCommand)

    except Exception as e:
        print(str(e))

ORG_EMAIL   = "@gmail.com"
FROM_EMAIL  = localIni.GetEmailUserName() + ORG_EMAIL
FROM_PWD    = localIni.GetEmailPassword()
SMTP_SERVER = "imap.gmail.com"
SMTP_PORT   = 993


