import sys
sys.path.insert(0, 'Support')

import urllib.request
import time
from html.parser import HTMLParser

import SendGmail
import localIni

strFinalMessage = ''
CompanyInfo = {}
bToday = False

class MyHTMLParser(HTMLParser):

    bLastSignalFound = False
    bLastCloseFound = False
    bChangeFound = False
    bChangePercentFound = False
    bLastSignalDate = False
    bFirstTableElementFound = False

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
                if b == 'MainContent_signalpagehistory_PatternHistory24_DXDataRow0':
                    self.bLastSignalDate = True

                    

    def handle_data(self, data):
        global strFinalMessage
        if self.bLastSignalFound:
            CompanyInfo['LastSignal'] = data
            self.bLastSignalFound = False
        if self.bLastCloseFound:
            CompanyInfo['LastClose'] = data
            self.bLastCloseFound = False
        if self.bChangeFound:
            CompanyInfo['Change'] = data
            self.bChangeFound = False
        if self.bChangePercentFound:
            CompanyInfo['PercentChange'] = data
            self.bChangePercentFound = False
        if self.bFirstTableElementFound:
            CompanyInfo['DateOfSignal'] = data
            self.bFirstTableElementFound = False
            self.bLastSignalDate = False
        if self.bLastSignalDate:
            self.bFirstTableElementFound = True
      

def DisplayStatus(Company):
    url = "https://www.americanbulls.com/SignalPage.aspx?lang=en&Ticker=" + Company
    myPage = urllib.request.urlopen(url)
    parser = MyHTMLParser()
    parser.feed(myPage.read().decode("utf-8"))

def GetCompanyInfo(Signal):
    Info = CompanyInfo[Signal]
    return Info

def CreateMessage(Company):
    bToday = time.strftime("%m/%d/%Y") == CompanyInfo['DateOfSignal']
    global strFinalMessage
    strFinalMessage = ''
    if bToday:
        strFinalMessage = "**TODAY**\n"
    strFinalMessage += GetCompanyInfo('DateOfSignal')   + "\n"
    strFinalMessage += GetCompanyInfo('LastSignal')     + "\n"
    strFinalMessage += GetCompanyInfo('LastClose')      + "\n"
    strFinalMessage += GetCompanyInfo('Change')         + "\n"
    strFinalMessage += GetCompanyInfo('PercentChange')
    if bToday:
        strFinalMessage += "\n" + localIni.GetPreviousStockSignal(Company)
    localIni.StoreStockInfo(Company, CompanyInfo['LastSignal'])

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
        CreateMessage(Company)
        print(strFinalMessage)

print(strFinalMessage)
