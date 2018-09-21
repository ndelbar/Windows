
import sys
sys.path.insert(0, 'Support')

import ReadGmail
import MoveGmail
import localIni
import getpass, imaplib

ReadGmail.read_email_from_gmail()

##################################################################################
# Move Read Inbox into processed
##################################################################################
MoveGmail.MoveAllEmailsTo('PROCESSED')
