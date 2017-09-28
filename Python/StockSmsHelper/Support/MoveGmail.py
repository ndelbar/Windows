
import localIni
import getpass, imaplib

ORG_EMAIL   = "@gmail.com"
FROM_EMAIL  = localIni.GetEmailUserName() + ORG_EMAIL
FROM_PWD    = localIni.GetEmailPassword()

##################################################################################
# Move Read Inbox into processed
##################################################################################
def MoveAllEmailsTo(DESTFOLDER):
    box = imaplib.IMAP4_SSL('imap.gmail.com')
    box.login(FROM_EMAIL,FROM_PWD)
    box.select()
    while True:
        typ, data = box.search(None, 'ALL')
        if data[0] == b'':
            break
        for num in data[0].split():
            result = box.store(num, '+X-GM-LABELS', DESTFOLDER)
            mov, data = box.uid('STORE', num , '+FLAGS', '(\Deleted)')
            box.store(num, '+FLAGS', '\\Deleted')
            box.expunge()
        SELECT = box.select()
    box.close()
    box.logout()
