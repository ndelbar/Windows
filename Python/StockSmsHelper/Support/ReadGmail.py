import smtplib
import time
import imaplib
import email
import localIni
import AmericanBulls
import SendGmail
# -------------------------------------------------
#
# Utility to read email from Gmail Using Python
#
# ------------------------------------------------

strKeySubject = "SMSHELPME"
strKeyUseStocks = "USESTOCKS:"
strKeySendAll = "SENDALL"

def GetInfo(Key, msg):
    iSubjectStart = msg.find(strKeyUseStocks) + len(strKeyUseStocks)
    iSubjectStop = msg.find('\\',iSubjectStart)
    strCommand = msg[iSubjectStart:iSubjectStop]
        
def SendCourtesyEmail(ToEmail):
    MESSAGE = "Thanks for using Stock.Sms.Helper\r\nYour email has been received\r\n\r\nAs a reminder, the subject needs to have SmsHelpMe in the subject, and one of the following commands in the email body:\r\nUseStocks:<Comma Delimited List>\r\nSendAll"
    SendGmail.send_GMail(FROM_EMAIL,FROM_PWD, ToEmail, "Thanks for using Stock.Sms.Helper",MESSAGE)

def read_email_from_gmail():
    try:
        imapSession = imaplib.IMAP4_SSL(SMTP_SERVER)
        imapSession.login(FROM_EMAIL,FROM_PWD)
        imapSession.select('Inbox')
        typ, data = imapSession.search(None, 'ALL')

        if typ != 'OK':
            print('Error searching Inbox.')
            raise
    
        # Iterating over all emails
        for msgId in data[0].split():
            typ, messageParts = imapSession.fetch(msgId, '(RFC822)')
            if typ != 'OK':
                print('Error fetching mail.')
                raise
            emailBody = messageParts[0][1]
            try:
                mail = email.message_from_bytes(emailBody)
                msg = str(email.message_from_string(str(emailBody))).upper().replace(' ','')
                for part in mail.walk():
                    print(part.get('subject'))
                    SUBJECT = part.get('subject').upper().replace(' ','')
                    FROM = part.get('from').upper().replace(' ','')
                    break
                if (SUBJECT == strKeySubject):
                    strCommand = GetInfo(strKeyUseStocks,msg)
                    print(strCommand)
                    SendCourtesyEmail(FROM)
                    if strCommand != None:
                        localIni.AddList(FROM,strCommand)
                    if (msg.find(strKeySendAll) != -1):
                        AmericanBulls.AllRoutine(FROM)
        
            except:
                print('cant read email')
        imapSession.close()
        imapSession.logout()

    except Exception as e:
        print(str(e))

ORG_EMAIL   = "@gmail.com"
FROM_EMAIL  = localIni.GetEmailUserName() + ORG_EMAIL
FROM_PWD    = localIni.GetEmailPassword()
SMTP_SERVER = "imap.gmail.com"
SMTP_PORT   = 993


