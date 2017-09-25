
import sys
sys.path.insert(0, 'Support')

import ReadGmail
import localIni
import getpass, imaplib

ReadGmail.read_email_from_gmail()

ORG_EMAIL   = "@gmail.com"
FROM_EMAIL  = localIni.GetEmailUserName() + ORG_EMAIL
FROM_PWD    = localIni.GetEmailPassword()

##################################################################################
# Move Read Inbox into processed
##################################################################################
box = imaplib.IMAP4_SSL('imap.gmail.com')
box.login(FROM_EMAIL,FROM_PWD)
box.select()
typ, data = box.search(None, 'ALL')
for num in data[0].split():
    result = box.store(num, '+X-GM-LABELS', 'Processed')
    #result = m.store(emailid, '+FLAGS', '\\Deleted')
    mov, data = box.uid('STORE', num , '+FLAGS', '(\Deleted)')
    box.store(num, '+FLAGS', '\\Deleted')
    box.expunge()
box.close()
box.logout()
