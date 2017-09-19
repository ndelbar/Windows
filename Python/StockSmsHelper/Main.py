import sys
sys.path.insert(0, 'Support')

import urllib.request
import time
from html.parser import HTMLParser

import SendGmail
import localIni

strFinalMessage = ''

class MyHTMLParser(HTMLParser):

    bLastSignalFound = False
    bLastCloseFound = False
    bChangeFound = False
    bChangePercentFound = False
    
    def handle_starttag(self, tag, attrs):
        for a, b in attrs:
            if a == 'id':
                if b == 'MainContent_LastSignal':
                    self.bLastSignalFound = True
                if b == 'MainContent_LastClose':
                    self.bLastCloseFound = True
                if b == 'MainContent_Change':
                    self.bChangeFound = True
                if b == 'MainContent_ChangePercent':
                    self.bChangePercentFound = True
                    

    def handle_data(self, data):
        global strFinalMessage
        if self.bLastSignalFound:
            strFinalMessage += data + "\n"
            print("Encountered some data  : ", data)
            self.bLastSignalFound = False
        if self.bLastCloseFound:
            strFinalMessage += data + "\n"
            print("Last Close: ", data)
            self.bLastCloseFound = False
        if self.bChangeFound:
            strFinalMessage += data + "\n"
            print("Change: ", data)
            self.bChangeFound = False
        if self.bChangePercentFound:
            strFinalMessage += data + "\n"
            print("Percent Change: ", data)
            self.bChangePercentFound = False
      

def DisplayStatus(Company):
    url = "https://www.americanbulls.com/SignalPage.aspx?lang=en&Ticker=" + Company
    myPage = urllib.request.urlopen(url)
    parser = MyHTMLParser()
    parser.feed(myPage.read().decode("utf-8"))

MAIN_USER = localIni.GetEmailUserName()
MAIN_PASSWORD = localIni.GetEmailPassword()
MSG_START = "********\r" + time.strftime("%m/%d/%Y") + "\r********\r" + time.strftime("%A")

for User in localIni.GetClients():
    SendGmail.send_GMail(MAIN_USER, MAIN_PASSWORD, localIni.GetSMSEmail(User),'', MSG_START)
    Companies = localIni.GetCompanies(User).split(',')
    for Company in Companies:
        print(Company)
        strFinalMessage += Company + ': '
        DisplayStatus(Company)
        strFinalMessage = strFinalMessage[:-1]
        SendGmail.send_GMail(MAIN_USER, MAIN_PASSWORD, localIni.GetSMSEmail(User),'',strFinalMessage)
        strFinalMessage = ""


print(strFinalMessage)
